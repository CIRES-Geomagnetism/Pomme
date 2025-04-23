import os
import platform
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext

pkg_dir = os.path.join("pomme", "cpomme")
src_dir = os.path.join(pkg_dir, "src")

class build_ext(_build_ext):
    def build_extensions(self):
        super().build_extensions()

compile_args = ["-O2"]

if platform.system() == "Windows":
    compile_args = ["/O2"]

module = Extension("pomme.cpomme", 
    sources=[os.path.join(pkg_dir, "model.c"), os.path.join(pkg_dir, "pomme_wrapper.c"),
    os.path.join(src_dir, "geod2geoc.c"),
    os.path.join(src_dir, "gsm2geo.c"),
    os.path.join(src_dir, "int-ext.c"),
    os.path.join(src_dir, "magfdz.c"),
    os.path.join(src_dir, "plmbar.c"),
    os.path.join(src_dir, "pom.c"),
    os.path.join(src_dir, "sm2geo.c"),
    os.path.join(src_dir, "utils.c"),
    ],
    include_dirs=[src_dir, pkg_dir],
    extra_compile_args=compile_args)

setup(
    ext_modules=[module],
    cmdclass={'build_ext': build_ext},
)

