from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext

class build_ext(_build_ext):
    def build_extensions(self):
        super().build_extensions()

module = Extension("pomme", 
    sources=["src/pomme_wrapper.c","src/model.c",
    "src/lib/geod2geoc.c",
    "src/lib/gsm2geo.c",
    "src/lib/int-ext.c",
    "src/lib/magfdz.c",
    "src/lib/plmbar.c",
    "src/lib/pom.c",
    "src/lib/sm2geo.c",
    "src/lib/utils.c"],
    include_dirs=["src"],
    extra_compile_args=["-O2"])

setup(
    #name='pomme',
    #version='0.1.0',
    #description='Pomme python module',
    ext_modules=[module],
    cmdclass={'build_ext': build_ext},
)

