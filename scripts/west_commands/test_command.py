from west.commands import WestCommand
from west import log

class TestCommand(WestCommand):

    def __init__(self):
        super().__init__('test', 'A test command')

    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(self.name, help=self.help)
        return parser

    def do_run(self, args, unknown_args):
        log.inf('Test command executed!')
