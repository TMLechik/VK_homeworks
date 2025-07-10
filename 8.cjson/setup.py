from setuptools import Extension, setup

def main():
    setup(
        name="cjson",
        version ="0.0.1",
        description="parse and serialize json python",
        ext_modules=[
            Extension('cjson', ['cjson.cpp'])
        ]
    )

if __name__ == "__main__":
    main()