## What it does
Replaces addresses by known labels found with *readelf -a* dump is necessary 

## Usage
* compile<br \>
*mvn assembly:single*
* run<br \>
*readelf -a &gt; inElfDump* <br \>
*java -jar elf-mapper-0.0.1-SNAPSHOT-jar-with-dependencies.jar inElfDump inFile* <br \><br \>
All occurences of 0x[0-9a-zA-Z]{4} are compared against addresses found in the elf dump and replaced in the output file. The output file is stored in the same folder where *inFile* resides prefixed with "pretty-" as *pretty-inFile*

