from buildtools.commands.base import Command
from buildtools.config import ProjectConfig
from buildtools.providers.cmake import CMakeProvider
from buildtools.shell import Shell


class CompileCommand(Command):
    """Builds the project (must run bootstrap first)."""

    name = "compile"
    summary = "Build the project"

    def __init__(self, config: ProjectConfig, shell: Shell,
                 cmake: CMakeProvider):
        self.config = config
        self.shell = shell
        self.cmake = cmake

    def execute(self) -> None:
        cmake_path = self.cmake.ensure()

        print(f"\n=== Building ({self.config.build_type}) ===")
        self.shell.run([
            cmake_path,
            "--build", "--preset", self.config.cmake_preset,
            "--config", self.config.build_type,
            "-j", str(self.config.jobs),
        ])

        print("\nBuild complete.")
