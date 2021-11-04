# Godot Error Handler

Simple error handler module for Godot Engine 4

## Installation

Copy `godoterrorhandler` to the `godot/modules` folder. Then [compile Godot](https://docs.godotengine.org/en/stable/development/compiling/index.html)

## Usage

### Engine.cfg

```
[autoload]

errorhandler="*res://scripts/errorhandler.gd"
```

### Example

```gdscript
extends node

func _ready():
  GodotErrorHandler.connect('error_threw', self, 'error_handler')

func error_handler(dump = null):
  # Dump contains following informations
  #	dump["error"] name
  #	dump["error_descr"] description
  #	dump["source_file"] file
  #	dump["source_line"] line number
  #	dump["source_func"] function name
  #	dump["warning"] = true/false
  #	dump["callstack"] = callstack (only in debug)
  print(dump)
```

# License

[See LICENSE file](./LICENSE)