from typing import Any, NamedTuple


class D(NamedTuple):
    red: Any
    green: int


d = D(red=0, green=5)
print(d)
