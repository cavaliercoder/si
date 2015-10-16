# si

Cross-platform hardware, software and configuration inventory.

Si is a cross-platform system inventory tool designed to provide a tree of
inventory data which may be output into a number of common data formats
including XML, JSON and YAML.

__WARNING__ This project is currently unreleased and subject to instability and
frequent breaking changes.

The goals of this project are as follows:

* provide one tool and common schema for multiple platforms (currently
  targetting Mac OS X, Windows XP/2003+ and Linux 2.4+) to bring together the
  best parts of tools like lshw, dmidecode, facter, ohai, etc.
* use system native APIs to provide unabstracted, verbose inventory data (i.e.
  no WMI calls or pipes from other programs)
* maximize the interface to enable easy consumption by other tools and libraries
* minimize delay before output
