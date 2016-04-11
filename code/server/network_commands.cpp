#include "lib/assert.h"
#include "common/serialization.h"
#include "network_commands.h"

memsize SerializeDisconnectNetworkCommand(buffer Buffer) {
  network_command_type Type = network_command_type_disconnect;
  serializer S = CreateSerializer(Buffer);
  SerializerWrite(&S, &Type, sizeof(Type));
  return sizeof(Type);
}

network_command_type UnserializeNetworkCommandType(buffer Buffer) {
  Assert(Buffer.Length >= sizeof(network_command_type));
  return *(network_command_type*)Buffer.Addr;
}

memsize SerializeBroadcastNetworkCommand(const client_id *IDs, memsize IDCount, const buffer Message, buffer Out) {
  serializer S = CreateSerializer(Out);
  network_command_type Type = network_command_type_broadcast;
  SerializerWrite(&S, &Type, sizeof(Type));
  SerializerWriteMemsize(&S, IDCount);
  SerializerWriteMemsize(&S, Message.Length);
  SerializerWrite(&S, IDs, sizeof(client_id)*IDCount);
  SerializerWriteBuffer(&S, Message);
  return S.Position;
}

broadcast_network_command UnserializeBroadcastNetworkCommand(buffer Buffer) {
  serializer S = CreateSerializer(Buffer);
  network_command_type Type = *(network_command_type*)SerializerRead(&S, sizeof(network_command_type));
  Assert(Type == network_command_type_broadcast);

  broadcast_network_command Cmd;
  Cmd.ClientIDCount = SerializerReadMemsize(&S);
  Cmd.MessageLength = SerializerReadMemsize(&S);
  Cmd.ClientIDs = (client_id*)SerializerRead(&S, sizeof(client_id)*Cmd.ClientIDCount);
  Cmd.Message = SerializerRead(&S, Cmd.MessageLength);

  return Cmd;
}