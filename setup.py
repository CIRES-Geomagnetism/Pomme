from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext

class build_ext(_build_ext):
    def build_extensions(self):
        super().build_extensions()

module = Extension("pomme.cpomme", 
    sources=["pomme/cpomme/pomme_wrapper.c","pomme/cpomme/model.c",
    "pomme/cpomme/src/geod2geoc.c",
    "pomme/cpomme/src/gsm2geo.c",
    "pomme/cpomme/src/int-ext.c",
    "pomme/cpomme/src/magfdz.c",
    "pomme/cpomme/src/plmbar.c",
    "pomme/cpomme/src/pom.c",
    "pomme/cpomme/src/sm2geo.c",
    "pomme/cpomme/src/utils.c"],
    include_dirs=["pomme/cpomme/src", "pomme/cpomme"],
    extra_compile_args=["-O2"])

setup(
    #name='pomme',
    #version='0.1.0',
    #description='Pomme python module',
    ext_modules=[module],
    cmdclass={'build_ext': build_ext},
)

