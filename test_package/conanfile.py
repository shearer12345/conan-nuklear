from conans import ConanFile, CMake
import os


channel = os.getenv("CONAN_CHANNEL", "testing")
username = os.getenv("CONAN_USERNAME", "shearer12345")


class ConannuklearTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "nuklear/1.33.0@%s/%s" % (
        username, channel), "SDL2/2.0.5@shearer12345/testing", "glew/2.0.0@coding3d/stable"
    generators = "cmake"

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake "%s" %s' %
                 (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def imports(self):
        self.copy("*.dll", "bin", "bin")
        self.copy("*.dylib", "bin", "bin")

    def test(self):
        os.chdir("bin")
        self.run(".%sexample" % os.sep)
