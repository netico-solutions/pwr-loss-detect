
# NOTE:
# Constants values are from Linux/include/uapi/asm-generic/ioctl.h
#
import os
import array
import fcntl
import struct
import logging


class _IOCTL_Command(object):


    def __init__(self, base_address):
        if type(base_address) is int:
            self.base_address = base_address
        elif type(base_address) is str:
            self.base_address = ord(base_address)

        self._IOC_NRBITS     = 8
        self._IOC_TYPEBITS   = 8
        self._IOC_SIZEBITS   = 14
        self._IOC_DIRBITS    = 2

        self._IOC_NRMASK     = ((1 << self._IOC_NRBITS)   - 1)
        self._IOC_TYPEMASK   = ((1 << self._IOC_TYPEBITS) - 1)
        self._IOC_SIZEMASK   = ((1 << self._IOC_SIZEBITS) - 1)
        self._IOC_DIRMASK    = ((1 << self._IOC_DIRBITS)  - 1)

        self._IOC_NRSHIFT    = 0
        self._IOC_TYPESHIFT  = (self._IOC_NRSHIFT + self._IOC_NRBITS)
        self._IOC_SIZESHIFT  = (self._IOC_TYPESHIFT + self._IOC_TYPEBITS)
        self._IOC_DIRSHIFT   = (self._IOC_SIZESHIFT + self._IOC_SIZEBITS)

        self._IOC_NONE       = 0
        self._IOC_WRITE      = 1
        self._IOC_READ       = 2

        self.max_size = 0


    def _IOC(self, dir, type, nr, size):
        value = (dir  << self._IOC_DIRSHIFT)  | \
                (type << self._IOC_TYPESHIFT) | \
                (nr   << self._IOC_NRSHIFT)   | \
                (size << self._IOC_SIZESHIFT)

        return value


    def _IO(self, type, nr):
        return self._IOC(self._IOC_NONE, type, nr, 0)


    def _IOR(self, type, nr, size):
        return self._IOC(self._IOC_READ, type, nr, size)


    def _IOW(self, type, nr, size):
        return self._IOC(self._IOC_WRITE, type, nr, size)


    def _IORW(self, type, nr, size):
        return self._IOC(self._IOC_READ | self._IOC_WRITE, type, nr, size)


    def add_command(self, name, nr, struct, access = 'na'):
        if access == 'r':
            command = self._IOR(self.base_address, nr, struct.size)
        elif access == 'w':
            command = self._IOW(self.base_address, nr, struct.size)
        elif access == 'rw':
            command = self._IORW(self.base_address, nr, struct.size)
        elif access == 'na':
            command = self._IO(self.base_address, nr)
        else:
            raise ValueError

        self.max_size = max(self.max_size, struct.size)
        self.__dict__.update({name:command})


    def __getattr__(self, attr):
        value = self.__dict__.update(attr)

        return value

_st_int32 = struct.Struct('i')
_ioctl_define = ['PWRLOSS_READ', 3, _st_int32, 'r']
_WDIOC = _IOCTL_Command('R')
_WDIOC.add_command(*_ioctl_define)
buf = array.array('i', [5])
fd = os.open('/dev/pwrloss_device', os.O_RDONLY)
fcntl.ioctl(fd, _WDIOC.PWRLOSS_READ, buf, 1)
print(buf[0])
