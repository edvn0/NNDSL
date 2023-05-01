from pathlib import Path
import os
from argparse import Action, ArgumentParser, Namespace
import shutil
import subprocess
import enum
import sys
from typing import Any, Sequence

shell = sys.platform.startswith('win')
current_source_dir = os.path.dirname(os.path.realpath(__file__))


class BuildMode(enum.Enum):
    Debug = "Debug"
    RelWithDebInfo = "RelWithDebInfo"
    Release = "Release"
    MinSizeRel = "MinSizeRel"


class Generator(enum.Enum):
    Ninja = "Ninja"
    VisualStudio2022 = "VS"

class Target(enum.Enum):
    Library = "NNDSL"
    Example = "NNDSLExample"


class EnumAction(Action):
    """
    Argparse action for handling Enums
    """

    def __init__(self, **kwargs):
        # Pop off the type value
        enum_type = kwargs.pop("type", None)

        # Ensure an Enum subclass is provided
        if enum_type is None:
            raise ValueError(
                "type must be assigned an Enum when using EnumAction")
        if not issubclass(enum_type, enum.Enum):
            raise TypeError("type must be an Enum when using EnumAction")

        # Generate choices from the Enum
        kwargs.setdefault("choices", tuple(e.value for e in enum_type))

        super(EnumAction, self).__init__(**kwargs)

        self._enum = enum_type

    def __call__(self, parser: ArgumentParser, namespace: Namespace, values: str | Sequence[Any] | None, option_string: str | None=None):
        # Convert value back into an Enum
        value = self._enum(values)
        setattr(namespace, self.dest, value)


def __remove(path: str | Path):
    """ param <path> could either be relative or absolute. """
    if os.path.isfile(path) or os.path.islink(path):
        os.remove(path)  # remove the file
        return 0
    elif os.path.isdir(path):
        shutil.rmtree(path)  # remove dir and all contains
        return 0
    else:
        return -1


def remove_folder_in(base_folder: str, folder: str):
    folder_full_path = f"{base_folder}/{folder}"
    if not Path(folder_full_path).exists():
        print(f"'{folder_full_path}' did not exist")
        return

    out = __remove(folder_full_path)
    if out != 0:
        print("Could not remove folders")


def make_symlink(file_from: str, file_to: str):
    if Path(file_to).exists():
        __remove(file_to)

    p = Path(file_from)
    os.symlink(p.absolute(), Path(file_to).absolute())
    return 0


def generate_cmake(generator: Generator, build_folder: str, build_mode: BuildMode, build_tests: bool):
    generator_string = str(
        generator.value) if generator == Generator.Ninja else "Visual Studio 17 2022"

    compile_definitions = [
        f'-D CMAKE_BUILD_TYPE={build_mode.value}',
        f'-D BUILD_TESTING={build_tests}',
        f"-G {generator_string}",
    ]

    builds_args = [f"-B {build_folder}",
                   f"-S {current_source_dir}"]

    cmake_args = ['cmake'] + builds_args + compile_definitions

    out = subprocess.call(args=cmake_args, shell=shell)
    if out != 0:
        print("Could not configure")
        exit(out)


def build_cmake(build_folder: str, target: Target):
    if sys.platform.startswith('win'):
        shell = True
    else:
        shell = False

    cmake_args = ['cmake', '--build', build_folder,
                  '--target', target.value, '--parallel', "12"]

    out = subprocess.call(args=cmake_args, shell=shell)
    if out != 0:
        print("Could not build")
        exit(out)


def run_tests(build_folder: str):
    ctest_args = ['ctest', '-j10', '-C', 'Debug',
                  '--test-dir', build_folder, '--output-on-failure']

    out = subprocess.call(args=ctest_args, shell=shell)
    if out != 0:
        print("Tests failed")
        exit(out)


def run_app(build_folder: str, build_mode: BuildMode, has_build_type_inside_build_folder: bool):
    args = ['NNDSLExample.exe']
    cwd_suffix = f'example/{build_mode.value}' if has_build_type_inside_build_folder else 'example'

    out = subprocess.call(args=args, shell=shell,
                          cwd=f"{build_folder}/{cwd_suffix}")
    if out != 0:
        print("Fault")
        exit(out)


def main(args: Namespace):
    print(args)

    generator: Generator = args.generator
    build_mode: BuildMode = args.mode
    build_folder = f"build-{build_mode.value}-{generator.value.replace(' ', '')}"

    if args.clean:
        remove_folder_in(build_folder, f'{build_mode.value}')

    build_folder_exists = Path(build_folder).exists()
    if not build_folder_exists or args.force_configure:
        generate_cmake(generator, build_folder, build_mode, args.build_tests)


    target: Target = args.target
    build_cmake(build_folder, target)

    if args.build_tests and build_mode == BuildMode.Debug:
        run_tests(build_folder)

    has_build_type_inside_build_folder = any([Path(f"{build_folder}/{x.name}").exists() for x in BuildMode])
    if args.run:
        run_app(build_folder, build_mode, has_build_type_inside_build_folder)

    if args.generate_symlink and Path(f"{build_folder}/compile_commands.json").exists():
        make_symlink(f"{current_source_dir}/{build_folder}/compile_commands.json", f"{current_source_dir}/compile_commands.json")


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('-t', '--target', type=Target,
                        action=EnumAction, default=Target.Example)
    parser.add_argument('-r', "--run", action='store_true')
    parser.add_argument('-b', '--build-tests', action='store_true')
    parser.add_argument('-f', '--force-configure', action='store_true')
    parser.add_argument('-c', '--clean', action='store_true')
    parser.add_argument('-s', '--generate-symlink', action='store_true')
    parser.add_argument('-g', '--generator', type=Generator,
                        action=EnumAction, default=Generator.Ninja)
    parser.add_argument('-m', '--mode', type=BuildMode,
                        action=EnumAction, default=BuildMode.Debug)

    main(parser.parse_args())
