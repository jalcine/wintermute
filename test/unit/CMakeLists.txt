###############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
###
### This file is part of Wintermute, the Extensible AI platform.
###
### Wintermute is free software; you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation; Either version 3 of the License, or
### (at your option) any later version.
###
### Wintermute is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without Even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

wintermute_test_render ( TARGET version SOURCES version.cpp )
wintermute_test_render ( TARGET arguments SOURCES arguments.cpp )
wintermute_test_render ( TARGET plugin SOURCES plugin.cpp )
wintermute_test_render ( TARGET plugin_process SOURCES plugin_process.cpp )
wintermute_test_render ( TARGET factory SOURCES factory.cpp )
wintermute_test_render ( TARGET logging SOURCES logging.cpp )
wintermute_test_render ( TARGET procedure_message SOURCES Procedure/message.cpp )
wintermute_test_render ( TARGET procedure_call SOURCES Procedure/call.cpp )
wintermute_test_render ( TARGET procedure_method_call SOURCES Procedure/method_call.cpp )
wintermute_test_render ( TARGET procedure_reply_call SOURCES Procedure/reply_call.cpp )
wintermute_test_render ( TARGET procedure_module_call SOURCES Procedure/module_call.cpp )
wintermute_test_render ( TARGET procedure_lambda_call SOURCES Procedure/module_call.cpp )
wintermute_test_render ( TARGET procedure_module_def SOURCES Procedure/module_def.cpp )
wintermute_test_render ( TARGET procedure_module SOURCES Procedure/module.cpp )
wintermute_test_render ( TARGET procedure_dispatcher SOURCES Procedure/dispatcher.cpp )
wintermute_test_render ( TARGET procedure_receiver SOURCES Procedure/receiver.cpp )
wintermute_test_render ( TARGET event_message SOURCES Events/message.cpp )
wintermute_test_render ( TARGET event_filter SOURCES Events/message_filter.cpp )
