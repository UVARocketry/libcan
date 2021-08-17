from pathlib import Path
from cffi import FFI


def main():
    ffi = FFI()

    # __file__ contains the path to this code
    header_path = Path(__file__).parent / Path('can_messages.h')
    # header_cache_path = Path(__file__).parent / Path('can_messages.h.cache')

    header_contents = open(header_path).read()
    # cffi does not process compiler directives (#include, etc.)
    # so comment them out
    header_contents = header_contents.replace('#', '//#')

    # packed to match struct alignment on RCU microcontrollers
    ffi.set_source('lib', None)
    ffi.cdef(header_contents, packed=True)
    ffi.compile()

    def bytes_to_struct(data_bytes: bytes, struct_name: str):
        struct = ffi.new(struct_name+'*')
        ffi.buffer(struct)[:] = data_bytes
        return struct

    def struct_to_bytes(data_struct, struct_name: str) -> bytes:
        return ffi.buffer(data_struct)[:]

    # Make up some data (of the right length)
    data = '\x42' * ffi.sizeof('struct Heartbeat_t')
    data = data.encode('utf-8')

    hb = bytes_to_struct(data, 'struct Heartbeat_t')
    print(hb)
    print(struct_to_bytes(hb, 'struct Heartbeat_t'))


if __name__ == '__main__':
    main()
