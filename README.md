# uhd-java

Java wrapper library for the [USRP Hardware Driver](http://code.ettus.com/redmine/ettus/projects/uhd/wiki)
via [JavaCPP](https://github.com/bytedeco/javacpp). Don't use this if
you don't know what you're doing, dismantle the state, fight the police.

## Install UHD
Install UHD and the UHD development files on your system, for debian
these can be found in the `uhd-host` and `libuhd-dev` packages. Check
out [this guide](http://code.ettus.com/redmine/ettus/projects/uhd/wiki#Installation-Instructions)
for detailed install instructions.

## Verify UHD
Connect your USRP hardware and run `$ uhd_find_devices`, **you must
be able to do this without root permissions**. The output should look
something like this:

```
linux; GNU C++ version 4.8.2; Boost_105400; UHD_003.005.005-0-unknown

--------------------------------------------------
-- UHD Device 0
--------------------------------------------------
Device Address:
    type: b100
    name:
    serial: ABC123DEF
```

## Build configuration
uhd-java contains integration tests that must be run on your actual
USRP hardware to verify proper functionality of the library. Copy the
file `example-hardware.properties` to another file named `hardware.properties`
and edit it as follows:

* **device_args** - address that uniquely identifies your device
* **clock_source** - clock source that will be used for testing
* **rx_antenna** - antenna that will be used to receive samples for testing
* **sub_device** - sub device spec to use for testing
* **rx_freq1/2** - two arbitrary frequencies that your USRP is capable of tuning to

## Build and install
```
$ mvn install
```
If all goes well you will end up with a file `target/uhd-java-<version>.jar`,
this contains all the compiled classes and shared object libraries.

## License

Copyright 2015 An Honest Effort LLC

Licensed under the GPLv3: http://www.gnu.org/licenses/gpl-3.0.html
