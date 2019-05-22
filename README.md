# EDArduino
Basic implementation of the Embedded Debugger on Embedded side.
This library is intended for the Arduino and should allow the user to implement the ED very easily.

For the current implementation of the EmbeddedDebugger, see https://github.com/DEMCON/EDApplication-csharp
For the current implementation of the EmbeddedDebugger Protocol, see https://demcon.github.io/EDProtocol/

TODOS:
- Properly finish the QueryRegister, to allow actual register values to be queried
- Setup channels
- Send channel data periodically
- Rewrite the current code, to make sure actual code is in .c/.cc/.cpp files
- Implement the DebugString command
- Implement ResetTime command
- Add all functionality to the WriteRegister, to allow for relative addresses and others
- Consider changing the object oriented to structs/plain c code
