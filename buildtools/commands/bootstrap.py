from buildtools.commands.base import Command
from buildtools.config import ProjectConfig
from buildtools.providers.cmake import CMakeProvider
from buildtools.providers.vcpkg import VcpkgProvider
from buildtools.shell import Shell
from buildtools.venv_manager import VenvManager


class BootstrapCommand(Command):
    """Downloads tools, installs vcpkg dependencies, configures the project."""

    name = "bootstrap"
    summary = "Install dependencies and configure the project"

    def __init__(self, config: ProjectConfig, shell: Shell,
                 venv_mgr: VenvManager, cmake: CMakeProvider,
                 vcpkg: VcpkgProvider):
        self.config = config
        self.shell = shell
        self.venv_mgr = venv_mgr
        self.cmake = cmake
        self.vcpkg = vcpkg

    def execute(self) -> None:
        self.venv_mgr.ensure()
        cmake_path = self.cmake.ensure()
        self.vcpkg.ensure()

        print(f"\n=== Configuring ({self.config.build_type}) ===")
        self.shell.run([
            cmake_path,
            "--preset", self.config.cmake_preset,
            "-S", self.config.project_dir,
        ])

        print(f"\nBootstrap complete.")
        print(f"  Dependencies : {self.config.deps_dir}")
        print(f"  Virtual env  : {self.config.venv_dir}")
        flag = " --release" if self.config.release else ""
        print(f"Run `python bootstrap.py compile{flag}` to build the project.")
