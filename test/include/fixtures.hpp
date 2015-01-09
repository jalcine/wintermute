/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <wintermutecore/module.hpp>
#include <wintermutecore/tunnel.hpp>
#include <wintermutecore/logging.hpp>
#include <wintermutecore/message.hpp>
#include <wintermutecore/call.hpp>
#include <string>

using std::string;

// TODO: Compile a list of faux values.

// Generates a random designation.
Wintermute::Module::Designation craftRandomDesignation()
{
  return Wintermute::Module::Designation("foo", "bar");
}

Wintermute::Message craftRandomMessage()
{
  Wintermute::Message::HashType data;
  data.insert(std::make_pair("foo", "bar"));
  return Wintermute::Message(data, craftRandomDesignation(), craftRandomDesignation());
}

class SampleModule : public Wintermute::Module
{
public:
  explicit SampleModule(const unsigned int index = 1) :
    Wintermute::Module(
      Wintermute::Module::Designation("input", "test" + std::to_string(index) + ".wintermute.in")
    )
  {
    winfo("SampleModule: My name is " + (string) designation());
  }
};

class SampleDispatcher : public Wintermute::Tunnel::Dispatcher
{
public:
  explicit SampleDispatcher(const string& theName = "sample") : Wintermute::Tunnel::Dispatcher(theName) { }
  virtual ~SampleDispatcher() { }

  virtual bool send (const Wintermute::Message& sendingMessage)
  {
    message = sendingMessage;
    return message == sendingMessage;
  }

  Wintermute::Message message;
};

class SampleReceiver : public Wintermute::Tunnel::Receiver
{
public:
  explicit SampleReceiver(const Wintermute::Message& storedMessage, const string& theName = "sample")
    : Wintermute::Tunnel::Receiver(theName), message(storedMessage) { }

  virtual ~SampleReceiver() { }

  virtual Wintermute::Message receive ()
  {
    return message;
  }

  Wintermute::Message message;
};

class SampleVoidCall : public Wintermute::Call
{
public:
  explicit SampleVoidCall() : Wintermute::Call("sampleVoid")
  {
    Call::Function func = [ = ](const string & arguments) -> const string
    {
      assert(arguments == arguments);
      return string();
    };

    bindFunction(func);
  }
  virtual ~SampleVoidCall() { }
};

class SampleCallWithValue : public Wintermute::Call
{
private:
  string val;
public:
  // TODO Fix this shit. Keep getting '-Wwrite-strings'.
  static const string DefaultValue;

  explicit SampleCallWithValue(const string& value = DefaultValue) :
    Wintermute::Call("samplevalue")
  {
    Call::Function func = [&value](const string & arguments) -> const string
    {
      assert(arguments == arguments);
      return value;
    };

    bindFunction(func);
  }
  virtual ~SampleCallWithValue() { }
};

class SampleVoidModuleCall : public Wintermute::Module::Call
{
public:
  explicit SampleVoidModuleCall(const Wintermute::Module::Ptr& modulePtr) :
    Wintermute::Module::Call(modulePtr, "sampleVoid")
  {
    Call::Function func = [](const string & arguments) -> const string
    {
      assert(arguments == arguments);
      return string();
    };

    bindFunction(func);
  }
  virtual ~SampleVoidModuleCall() { }
};

class SampleModuleCallWithValue : public Wintermute::Module::Call
{
public:
  explicit SampleModuleCallWithValue(const Wintermute::Module::Ptr& modulePtr,
      const string& value = string(SampleCallWithValue::DefaultValue)) :
    Wintermute::Module::Call(modulePtr, "sampleWithValues")
  {
    Call::Function func = [ &value ](const string & arguments) -> const string
    {
      assert(arguments == arguments);
      return value;
    };

    bindFunction(func);
  }
  virtual ~SampleModuleCallWithValue() { }
};

class SampleMimicModuleCall : public Wintermute::Module::Call
{
public:
  explicit SampleMimicModuleCall(const Wintermute::Module::Ptr& modulePtr) :
    Wintermute::Module::Call(modulePtr, "sampleMimic")
  {
    Call::Function func = [](const string & arguments) -> const string
    {
      assert(arguments == arguments);
      return arguments;
    };

    bindFunction(func);
  }
  virtual ~SampleMimicModuleCall() { }
};

class SampleEvent : public Wintermute::Events::Event
{
public:
  explicit SampleEvent(const string& eventName) : Event(eventName) { }
};

const string SampleCallWithValue::DefaultValue = string("sample");
