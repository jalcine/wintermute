#include "message_serializer.hh"
#include "message_serializer.hpp"
#include "message.hpp"
#include "module.hpp"
#include <json/json.h>

using Wintermute::Module;
using Wintermute::Message;
using Wintermute::MessageSerializer;

W_DECLARE_SINGLETON(MessageSerializer);

const Module::Designation import_designation(const Json::Value& json)
{
  return Module::Designation(json["name"].asString(), json["domain"].asString(), json["pid"].asUInt());

}

void import_payload(const Json::Value& json, Message& message)
{
  Message::HashType aMap;
  for (const string memberName : json.getMemberNames())
  {
    const string memberValue = json[memberName].asString();
    aMap.insert(std::make_pair(memberName, memberValue));
  }

  message.setPayload(aMap);
}

Json::Value serialize_designation(const Module::Designation& designation)
{
  Json::Value designationJson;
  designationJson["name"] = designation.name();
  designationJson["domain"] = designation.domain();
  designationJson["pid"] = designation.pid();

  return designationJson;
}

MessageSerializer::MessageSerializer()
{
}

Message MessageSerializer::deserialize(const string& data)
{
  Message theMessage;
  Json::Value messageJson;
  Json::Reader jsonReader;
  const bool wasReadSuccessful = jsonReader.parse(data, messageJson);

  if (wasReadSuccessful)
  {
    Json::Value senderJson = messageJson["sender"];
    Json::Value receiverJson = messageJson["receiver"];
    Json::Value payloadJson = messageJson["payload"];

    theMessage.setSender(import_designation(senderJson));
    theMessage.setReceiver(import_designation(receiverJson));
    import_payload(payloadJson, theMessage);
  }

  return theMessage;
}

const string MessageSerializer::serialize(const Message& message)
{
  Json::Value messageJson;
  Message::HashType payload = message.payload();
  messageJson["sender"] = serialize_designation(message.sender());
  messageJson["receiver"] = serialize_designation(message.receiver());
  messageJson["payload"] = Json::Value();

  for (auto itr = payload.begin(); itr != payload.end(); itr++){
    messageJson["payload"][itr->first] = itr->second;
  }

  Json::FastWriter jsonWriter;
  return jsonWriter.write(messageJson);
}

MessageSerializer::~MessageSerializer()
{
}
