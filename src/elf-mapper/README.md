elf mappper
===========
Replaces address occurences matching **0x[0-9a-zA-Z]{4}**  by labels as shown by **readelf -a**.

## Usage
    java -jar <mapper> <labelsFile> <fileToReplaceAddresses>
Writes the translated file to **pretty-fileToReplaceAddresses**.
    
###Compile

    mvn assembly:single*
    
### Run

    readelf -a > elfDump.txt
    java -jar ./target/elf-mapper-0.0.2-SNAPSHOT-jar-with-dependencies.jar elfDump.txt fileToReplaceAddresses.txt
