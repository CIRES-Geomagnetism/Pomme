from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext

class build_ext(_build_ext):
    def build_extensions(self):
        super().build_extensions()

module = Extension("pomme", 
    sources=["pomme/pomme_wrapper.c","pomme/model.c",
    "pomme/src/geod2geoc.c",
    "pomme/src/gsm2geo.c",
    "pomme/src/int-ext.c",
    "pomme/src/magfdz.c",
    "pomme/src/plmbar.c",
    "pomme/src/pom.c",
    "pomme/src/sm2geo.c",
    "pomme/src/utils.c"],
    include_dirs=["pomme/src", "pomme"],
    extra_compile_args=["-O2"])

setup(
    #name='pomme',
    #version='0.1.0',
    #description='Pomme python module',
    ext_modules=[module],
    cmdclass={'build_ext': build_ext},
)

