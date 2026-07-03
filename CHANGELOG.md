# Changelog

## 0.3.1

Maintenance release for metadata, compatibility, and small bug fixes.

### Changed

* Updated Arduino and PlatformIO metadata to version `0.3.1`.
* Declared RP2040 support in Arduino metadata.
* Added `return_Brightness()` as the correctly spelled brightness helper.
* Kept `return_Brigtness()` as a backward-compatible alias.
* Restored JSON output in `print_color_json()`.
* Updated README API notes and supported architectures.

### Fixed

* Fixed `init()` returning `-1` after successful initialization.
* Fixed `print_color_json()` output to produce valid JSON without a trailing comma.
* Updated stale return-value comments for color helper methods.

## 0.3.0

### Changed

* Changed `print_color()` to return the detected hue value.
* Added `return_Brigtness()` for returning brightness/value data.
* Changed `print_color_json()` to return the detected hue value.
* Disabled default `printf`/stdout debug output in color reading, initialization, and example sketches.
* Updated Arduino library metadata to version `0.3.0`.
* Updated library description and keywords.
* Improved README image formatting.
