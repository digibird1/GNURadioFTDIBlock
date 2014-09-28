GNURadioFTDIBlock
=================

(c) by Daniel Pelikan

This is the source code for a GNU Radio source block which reads from an FTDI USB2.0 controler.
It was tested with the FTDI FT232H Single Channel Hi-Speed USB to Multipurpose UART/FIFO IC.

The package depends on the libftdi library, included in this repositorty.

I the example folder is an example how to use this block.

It can simply be downloaded and installed via:

INSTALL_PATH=$HOME/tmp/GnuRadioModuleTest
GNURadio_PATH=$HOME/bin/GnuRadioInstall

cd libftdi1-1.1
mkdir build
cd build/
cmake ../ -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
make
make install
cd ../..

export PATH=$PATH:$GNURadio_PATH
export PATH=$PATH:$GNURadio_PATH/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GNURadio_PATH/lib
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$GNURadio_PATH/lib/pkgconfig
export PYTHONPATH=$PYTHONPATH:$GNURadio_PATH/lib/python2.7/dist-packages
export GRC_BLOCKS_PATH=$GNURadio_PATH/share/gnuradio/grc/blocks:$GRC_BLOCKS_PATH
export PATH=$PATH:$INSTALL_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_PATH/lib
export PYTHONPATH=$PYTHONPATH:$INSTALL_PATH/lib/python2.7/dist-packages

cd gr-FTxxxRead
mkdir build
cd build/
cmake ../ -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
make
make install

cd ../..

export GRC_BLOCKS_PATH=$INSTALL_PATH/share/gnuradio/grc/blocks:$GRC_BLOCKS_PATH

gnuradio-companion gr-FTxxxRead/examples/GNURadioFTDITest.grc
