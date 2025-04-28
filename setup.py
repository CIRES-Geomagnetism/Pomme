from pathlib import Path
import os,platform
from setuptools import setup, Extension

pkg_dir = os.path.join("pomme", "cpomme")
src_dir = os.path.join(pkg_dir, "src")

compile_args = ["-O2"]

if platform.system() == "Windows":
    compile_args = ["/O2"]

module = Extension(name="pomme.cpomme", 
    sources=[
            os.path.join(pkg_dir, "model.c"), 
            os.path.join(pkg_dir, "pomme_wrapper.c"),
            os.path.join(src_dir, "geod2geoc.c"),
            os.path.join(src_dir, "gsm2geo.c"),
            os.path.join(src_dir, "int-ext.c"),
            os.path.join(src_dir, "magfdz.c"),
            os.path.join(src_dir, "plmbar.c"),
            os.path.join(src_dir, "pom.c"),
            os.path.join(src_dir, "sm2geo.c"),
            os.path.join(src_dir, "utils.c"),
            ],
    include_dirs=[src_dir,pkg_dir],
    extra_compile_args=compile_args)

setup(
    packages=["pomme"],
    ext_modules=[module]
)

