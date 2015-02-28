package at.tugraz.iti.programmablematter.elfmapper;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.ObjectInputStream.GetField;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.LineIterator;

public class ElfAddresToNameTranslator {

	private File elfDumpLookup;
	private File inFileToTranslate;
	private File translatedOutFile;

	private Set<String> knwonTypes = new HashSet<String>();

	public ElfAddresToNameTranslator(String[] args) throws Exception {
		elfDumpLookup = new File(args[0]);
		inFileToTranslate = new File(args[1]);

		knwonTypes.add("NOTYPE");
		knwonTypes.add("FUNC");
	}

	Map<String, Set<String>> conversionTale() throws IOException {
		LineIterator line = FileUtils.lineIterator(elfDumpLookup);
		// Num: Value Size Type Bind Vis Ndx Name
		// 0: 00000000 0 NOTYPE LOCAL DEFAULT UND NAME
		Pattern pattern = Pattern.compile("[0-9a-fA-F]{8}");
		HashMap<String, Set<String>> addressToLabel = new HashMap<String, Set<String>>();

		while (line.hasNext()) {
			try {
				String[] values = line.nextLine().replaceAll("[ ]+", " ")
						.split(" ");
				Matcher matcher = pattern.matcher(values[2]);
				// line must contain address
				if (!matcher.matches()) {
					continue;
				}

				String type = values[4];
				// line must contain a known type
				if (!knwonTypes.contains(type)) {
					continue;
				}

				String address = values[2];
				String name = values[8];

				Set<String> knownAddressNames = addressToLabel.get(address
						.toLowerCase());
				if (knownAddressNames == null) {
					knownAddressNames = new HashSet<String>();
					knownAddressNames.add(name);
					addressToLabel
							.put(address.toLowerCase(), knownAddressNames);
				} else {
					knownAddressNames.add(name);
				}

			} catch (IndexOutOfBoundsException e) {
				continue;
			}
		}
		line.close();
		return addressToLabel;
	}

	public void convert() throws IOException {
		System.out.println("process dump <" + elfDumpLookup.getName()
				+ "> and dot <" + inFileToTranslate.getName() + ">");

		Map<String, Set<String>> conversionTable = conversionTale();

		translatedOutFile = new File(inFileToTranslate.getParent() + "/pretty-"
				+ inFileToTranslate.getName());
		translatedOutFile.delete();
		if (!translatedOutFile.createNewFile()) {
			throw new IOException("cannot create new file");
		}

		PrintWriter fileWriter = new PrintWriter(translatedOutFile);

		LineIterator line = FileUtils.lineIterator(inFileToTranslate);
		while (line.hasNext()) {
			fileWriter.write(renameAddresses(conversionTable, line.nextLine()
					.toLowerCase()));
		}

		fileWriter.close();
		System.out.println("wrote file to " + translatedOutFile.getPath());
	}

	private String renameAddresses(Map<String, Set<String>> conversionTable,
			String line) {
		Pattern addressPattern = Pattern.compile("0x[A-Fa-f0-9]{4}");
		Matcher matcher = addressPattern.matcher(line);

		while (matcher.find()) {
			String address = matcher.group().toLowerCase();
			String lookUpKey = address.replace("0x", "0000");// .replace(" -",
																// "");
			System.out.print("inspect " + address + " " + lookUpKey + " -> ");
			Set<String> names = conversionTable.get(lookUpKey);

			if (names != null) {
				String name = null;
				if (names.size() <= 1) {
					name = names.iterator().next();
				} else {
					name = "{" + joinSet(names) + "}";
				}
				System.out.println(name);
				line = Pattern.compile(address).matcher(line).replaceAll(name);
			} else {
				System.out.println();
			}
		}
		return line;
	}

	public static String joinSet(Set<String> values) {
		String[] concatenated = new String[values.size()];
		int idx = 0;
		for (String value : values) {
			concatenated[idx++] = value;
		}
		return String.join(", ", concatenated);
	}

	private static void usage() {

		String name = "<your>.jar";
		try {
			name = new File(ElfAddresToNameTranslator.class
					.getProtectionDomain().getCodeSource().getLocation()
					.toURI().getPath()).getName();
		} catch (Exception e) {
		}

		System.out.println("\n\n");
		System.out.println("Usage");
		System.out.println("java -jar " + name
				+ " <elfDumpFile> <fileToTranslate>");
		System.out
				.println("the converted result is stored in same folder as <fileToTranslate> named pretty-<fileToTranslate>");
		System.out.println("\n");

	}

	public static void main(String[] args) throws IOException {
		ElfAddresToNameTranslator r = null;
		try {
			r = new ElfAddresToNameTranslator(args);
			try {
				for (Map.Entry<String, Set<String>> entry : r.conversionTale()
						.entrySet()) {
					System.out.println(entry.getKey() + "-> {"
							+ joinSet(entry.getValue()) + "}");
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			r.convert();
		} catch (Exception e) {
			usage();
		}
	}
}
