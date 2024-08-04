# my_west_extension.py
import os
import sys
import subprocess
from textwrap import dedent
from west.commands import WestCommand
from west import log
from west.manifest import Manifest
from west.util import WestNotFound

def get_west_workspace_root():
    try:
        manifest = Manifest.from_file()
        return manifest.topdir
    except WestNotFound:
        log.err("Error: Not inside a west workspace. Please run this within a west-managed directory.")
        sys.exit(1)

class Console(WestCommand):
    def __init__(self):
        super().__init__(
            'console',  # Command name
            'Open RoboIME console',
            dedent('''
                A multi-line description of my-command.
                You can split this up into multiple paragraphs and they'll get
                reflowed for you. You can also pass
                formatter_class=argparse.RawDescriptionHelpFormatter when calling
                parser_adder.add_parser() below if you want to keep your line
                endings.'''))

    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(self.name, help=self.help)
        parser.add_argument('args', nargs='*', help='Arguments to pass to console.sh')
        return parser

    def do_run(self, args, unknown_args):
        # Get the workspace root directory
        workspace_root = get_west_workspace_root()

        # Path to the script relative to the workspace root
        script_path = os.path.join(workspace_root, 'ssl_firmware/scripts/west_commands/console.sh')

        if not os.path.exists(script_path):
            print(f"Error: {script_path} not found.")
            return 1

        # Make sure the script is executable
        os.chmod(script_path, 0o755)

        # Execute the script with additional arguments
        command = [script_path] + args.args  # Append any additional arguments passed
        result = subprocess.run(command, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"Error executing {script_path}: {result.stderr}")
            return result.returncode

        print(f"Script output:\n{result.stdout}")
        return 0
