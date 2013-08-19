# Import some pretty printers for Qt.
# link: http://nikosams.blogspot.com/2009/10/gdb-qt-pretty-printers.html
python
import sys
sys.path.insert(0, "./.gdb/python")
from qt4 import register_qt4_printers
register_qt4_printers(None)
end

# Enable pretty printing.
set print pretty 1

# Load the binary file.
file build/src/wintermute

## Define some breakpoints.
break main
break Wintermute::Application::start()
break Wintermute::Application::stop()
