avr 
---
The particle firmware development project folder.

elf-mapper
-------------
A helper replacing addresses by labels.

sample-visualization
--------------------
Python log file visualization for analysis and demonstration.

structs-to-json
---------------
An autogenerator that generates a JSON description out of the firmware's C source code. 
The description file is used by the simulation's particle platform monitor to translate 
writes onto registers of interrest.


Example 
-------
Exemplary out fragment of the particle firmware implementation:

    {
      "enums": {
        [...]
        "NodeType": [
          "NODE_TYPE_INVALID",
          "NODE_TYPE_ORPHAN",
          "NODE_TYPE_ORIGIN",
          "NODE_TYPE_INTER_HEAD",
          "NODE_TYPE_INTER_NODE",
          "NODE_TYPE_TAIL",
          "NODE_TYPE_MASTER"
        ],
      },
      "labels": {
        "globalStateBase": 96 /* global struct starts at this address*/
      },
      [...]
      "structs": {
        "Particle.node": [
          {
            "property": "state",
            "type": "StateType",
            "address": "globalStateBase+1"
          },
          {
            "property": "type",
            "type": "NodeType",
            "address": "globalStateBase+3"
          }
        ],
        "Particle.node.address": [
         {
            "property": "row",
            "type": "unsigned",
            "address": "globalStateBase+5"
          },
          {
            "property": "column",
            "type": "unsigned",
            "address": "globalStateBase+6"
          }
        ]
      }
    }
