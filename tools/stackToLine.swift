import Foundation

extension String {
    func colorfull(_ color: Int) -> String {
        return "\u{001B}[\(color)m\(self)\u{001B}[0m"
    }
}

let fileManager = FileManager.default

let toolDir = URL(fileURLWithPath: #file).deletingLastPathComponent()

let elfFilePath = toolDir
    .appendingPathComponent("../.pio/build/esp32dev/firmware.elf")
    .path

let toolchainLocation = fileManager
    .homeDirectoryForCurrentUser
    .appendingPathComponent(".platformio/packages/toolchain-xtensa-esp32")
    .path

if !fileManager.fileExists(atPath: elfFilePath) {
    fatalError("No elf file found at \(elfFilePath)")
} 

if !fileManager.fileExists(atPath: toolchainLocation) {
    fatalError("No toolchain directory found at \(toolchainLocation)")
} 

let projectDir = toolDir.deletingLastPathComponent().path

print("dir: \(projectDir), elfFilePath: \(elfFilePath), toolchainLocation: \(toolchainLocation)")

let addresses = CommandLine.arguments.dropFirst() // Get command-line arguments

print("addresses count: \(addresses.count)\n\n")

for address in addresses {
    let cmd = "\(toolchainLocation)/bin/xtensa-esp32-elf-addr2line -pfiaC -e \(elfFilePath) \(address)"
    let task = Process()
    task.launchPath = "/bin/bash"
    task.arguments = ["-c", cmd]
    let outputPipe = Pipe()
    task.standardOutput = outputPipe
    task.launch()
    task.waitUntilExit()

    let outputData = outputPipe.fileHandleForReading.readDataToEndOfFile()
    if let output = String(data: outputData, encoding: .utf8) {
        let parts = output.components(separatedBy: " at ")
        let firstLine = parts.first ?? ""
        let firstLineParts = firstLine.components(separatedBy: ": ")
        print("\(firstLineParts[0].colorfull(33)): \(firstLineParts[1].colorfull(32))")
        for part in Array(parts.dropFirst()) {
            print("\t\(part.colorfull(34))")
        }
         print("\n")
    }
}