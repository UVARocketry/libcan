from pyclibrary import CParser
from pathlib import Path

#__file__ contains the path to this 
path = Path(__file__).parent / Path('can_messages.h')

parser = CParser(files=(str(path)))

print(parser)
