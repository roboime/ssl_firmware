# scripts/west_commands/my_west_extension.py

from west.commands import WestCommand  # Subclass this for your extension
from west import log  # Use this for user output

class Console_Robo(WestCommand):

    def __init__(self):
        super().__init__(
            'console',  # This gets stored as self.name
            'Open RoboIME shell console',  # One-line help for what the command does
            '''
            This command opens the RoboIME shell console.
            It requires a required argument and can accept an optional one.
            ''',
        )
        
    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(
            self.name,
            help=self.help,
            description=self.description
        )

        # Add options using argparse module API.
        parser.add_argument('-o', '--optional', help='An optional argument')
        parser.add_argument('required', help='A required argument')

        return parser  # Gets stored as self.parser

    def do_run(self, args, unknown_args):
        print('Running the console command...')
        log.inf('--optional is', args.optional)
        log.inf('required is', args.required)
