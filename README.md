## UE4 SARL

Plugin for UE4 able to receive data emitted by SARL agents.

## Installation

Just clone this directory under the Plugins directory of your project.

## Serialization

This plugin relies on protobuf to perform serialization and de-serialization of data.
Some messages are built-in (influence, perception, vector, quaternion).
To include others, follow these instructions:

- Apply regenerateforue4.py on the .pb.cc files
- Wrap the include of the .ph.h headers within #include "AllowWindowsPlatformTypes.h and #include "HideWindowsPlatformTypes.h"