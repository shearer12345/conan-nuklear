from conans import ConanFile, tools
import os


class NuklearConan(ConanFile):
    name = "nuklear"
    version = "1.33.0"
    license = "public domain"
    url = "https://github.com/shearer12345/conan-nuklear"
    # No settings/options are necessary, this is header only

    description = """A single-header ANSI C gui library

    This is a minimal state immediate mode graphical user interface toolkit written in ANSI C and licensed under public domain. It was designed as a simple embeddable user interface for application and does not have any dependencies, a default renderbackend or OS window and input handling but instead provides a very modular library approach by using simple input state for input and draw commands describing primitive shapes as output. So instead of providing a layered library that tries to abstract over a number of platform and render backends it only focuses on the actual UI."""

    commitHash = "ef2dcd3b779647e0140bb78863cb8439774e277b"

    def source(self):
        '''retrieval of the source code here. Remember you can also put the code in the folder and
        use exports instead of retrieving it with this source() method
        '''
        self.run(
            "git clone https://github.com/vurtun/nuklear.git && cd nuklear && git reset --hard " + self.commitHash)

    def package(self):
        self.copy("*.h", dst="include", src="nuklear")
