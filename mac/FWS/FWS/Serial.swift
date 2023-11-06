import Foundation

class Serial {
    enum SerialError: Error {
        case unknown
    }

    enum Mode {
        case readonly
        case writeonly
        case readWrite

        var cMode: Int32 {
            switch self {
            case .readonly: return O_RDONLY
            case .writeonly: return O_WRONLY
            case .readWrite: return O_RDWR
            }
        }
    }

    let path: String
    private(set) var file: Int32?

    init(path: String) {
        self.path = path
    }

    func open(mode: Mode) throws -> Int32 {
        let cMode = mode.cMode
        let rVal = Darwin.open(path, cMode)
        guard rVal != -1 else {
            // TODO: Return proper error.
            throw SerialError.unknown
        }
        return rVal
    }
}
