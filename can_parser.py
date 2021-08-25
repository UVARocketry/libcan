from copy import Error
from pathlib import Path
from types import SimpleNamespace
from typing import NamedTuple
from can import Message
import cffi


class Error(Exception):
    pass


class InvalidCANFrameError(Error):
    pass


class UnsupportedCANFrameTypeError(InvalidCANFrameError):
    pass


class InvalidCANFrameIDError(InvalidCANFrameError):
    pass


class InvalidCANFrameLengthError(InvalidCANFrameError):
    pass


class CANParser:
    id_name_dict = dict()
    name_dict = dict()

    def msg_to_data(cls, msg: Message) -> SimpleNamespace:
        if msg.is_extended_id or msg.is_fd or msg.is_remote_frame or msg.is_error_frame:
            raise UnsupportedCANFrameTypeError

        msg_id = (msg.arbitration_id // 16) * 16
        rcu_id = msg.arbitration_id % 16
        try:
            msg_name = cls.id_name_dict[msg_id]
            temp = cls.name_dict[msg_name]
        except KeyError:
            raise InvalidCANFrameIDError

        _, msg_len, py_structtype, c_struct = temp

        if msg.dlc != msg_len or len(msg.data) != len:
            raise InvalidCANFrameLengthError

        cls.ffi.buffer(c_struct)[:] = msg.data
        fields_dict = {field: getattr(c_struct, field) for field in dir(c_struct)}

        return py_structtype(**fields_dict), rcu_id

    def data_to_msg(cls, data: NamedTuple) -> Message:
        

    def load_can_messages_header(cls):
        # __file__ contains the path to this code
        hfile_path = Path(__file__).parent / Path('can_messages.h')
        # header_cache_path = Path(__file__).parent / Path('can_messages.h.cache')

        with open(hfile_path) as f:
            hfile = f.read()

        # cffi does not process compiler directives (#include, etc.)
        # so comment them out
        hfile = hfile.replace('#', '//#')

        cls.ffi = cffi.FFI()
        # packed to match struct alignment on RCU microcontrollers
        cls.ffi.cdef(hfile, packed=True)

        search = '#define CAN_ID_'
        id_define_idxs = [i + len(search) for i in range(len(hfile))
                          if hfile.startswith(search, i)]
        id_defines = [hfile[i:].split('\n', 1)[0].split()
                      for i in id_define_idxs]

        with open('out.py', 'w') as f:
            f.writelines(('#Auto-generated file; do not edit.\n',
                          'from typing import NamedTuple\n',
                          'from typing import Any\n'))

            for define in id_defines:
                full_name = define[0]  # e.g. 'MotorStatus_OX_MAIN'
                short_name = define[0].split('_', 1)[0]  # 'MotorStatus'
                id = int(define[1], base=16)
                c_struct_name = f'struct {short_name}_t'
                py_struct_name = f'{short_name}Data'
                msg_len = cls.ffi.sizeof(c_struct_name)

                c_struct = cls.ffi.new(c_struct_name+'*')
                fields = dir(c_struct)
                f.write(f'\nclass {py_struct_name}(NamedTuple):\n')
                f.writelines([f'\t{field}: Any\n' for field in fields])

                cls.id_name_dict[id] = full_name
                cls.name_dict[full_name] = (
                    id, msg_len, py_struct_name, c_struct)

        import out
        for name in cls.name_dict:
            id, msg_len, py_struct_name, c_struct = cls.name_dict[name]
            py_struct_type = eval(f'out.{py_struct_name}')
            cls.name_dict[name] = (id, msg_len, py_struct_type, c_struct)


if __name__ == '__main__':
    parser = CANParser()
    parser.load_can_messages_header()
    print('RX:', parser.id_name_dict)
    print('TX:', parser.name_dict)
    pass
