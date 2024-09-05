from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext

class build_ext(_build_ext):
    def build_extensions(self):
        super().build_extensions()

module = Extension("pomme", 
    sources=["pomme/lib/pomme_wrapper.c","pomme/lib/model.c",
    "pomme/lib/src/geod2geoc.c",
    "pomme/lib/src/gsm2geo.c",
    "pomme/lib/src/int-ext.c",
    "pomme/lib/src/magfdz.c",
    "pomme/lib/src/plmbar.c",
    "pomme/lib/src/pom.c",
    "pomme/lib/src/sm2geo.c",
    "pomme/lib/src/utils.c"],
    include_dirs=["pomme/lib/src"])

setup(
    #name='pomme',
    #version='0.1.0',
    #description='Pomme python module',
    ext_modules=[module],
    cmdclass={'build_ext': build_ext},
)
