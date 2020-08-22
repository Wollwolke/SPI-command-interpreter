from setuptools import setup

setup(
    name="SPI-command-interpreter",
    version="0.1",
    packages=["spi_interpreter"],
    python_requires=">=3.6",
    entry_points={
        "console_scripts": ["spi_interpreter = spi_interpreter.__main__:main",],
    },
    install_requires=["cexprtk>=0.3.4"],
    package_data={"spi_interpreter": ["Registers.json"],},
    # metadata to display on PyPI
    author="Lars Wiesner",
    description="Models control register of a SPI controllable chip and interprets SPI commands based on a config file as readable text commands.",
)
