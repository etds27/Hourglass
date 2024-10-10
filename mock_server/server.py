import pprint
import aioconsole
import asyncio
from bleak import BleakScanner, BleakClient
from bleak.backends.device import BLEDevice
from bleak.uuids import normalize_uuid_16
from enum import Enum
from typing import List

SERVICE_UUID = "d7560343-51d4-4c24-a0fe-118fd9078144"

class CharacteristicUUID(str, Enum):
    num_players = "d776071e-9584-42db-b095-798a90049ee0"
    current_player = "6efe0bd2-ad04-49bb-8436-b7e1d1902fea"
    timer = "4661b4c1-093d-4db7-bb80-5b5fe3eae519"
    my_player = "f1223124-c708-4b98-a486-48515fa59d3d"
    my_turn = "c27802ab-425e-4b15-8296-4a937da7125f"
    skipped = "c1ed8823-7eb1-44b2-ac01-351e8c6a693c"
    game_active = "33280653-4d71-4714-a03c-83111b886aa7"

async def main():
    def int_to_bytes(value: int, len: int = 4) -> bytes:
        return (value).to_bytes(len, byteorder="little")

    def bool_to_bytes(value: bool, len: int = 1) -> bytes:
        return (value).to_bytes(len, byteorder="little")

    async def write_to_all_clients(uuid: CharacteristicUUID, value: bytearray):
        for client in clients:
            await client.write_gatt_char(uuid, value)
            
    num_players = 8
    current_player = 0

    # game_devices = [device for device in await BleakScanner.discover(service_uuids = [SERVICE_UUID])]
    # print(game_devices)


    devices = await BleakScanner.discover()
    pprint.pprint(devices)
    game_devices = [device for device in devices if device.name and device.name and device.address.startswith("48:27")]

    async with BleakClient(game_devices[0], timeout=60, services=[SERVICE_UUID]) as client:
        clients = [client]
        print(f"Setting up client: {client}")
        # Setup clients
        for i, client in enumerate(clients):
            print(f"Player Number: {i} | {CharacteristicUUID.my_player}")
            print(int_to_bytes(0))
            await client.write_gatt_char(CharacteristicUUID.my_player.value, int_to_bytes(0))
            print(f"Player turn  : {0}")
            await client.write_gatt_char(CharacteristicUUID.my_turn.value, bool_to_bytes(False))
            print(f"Number player: {num_players}")
            await client.write_gatt_char(CharacteristicUUID.num_players.value, int_to_bytes(8))
            print(f"current Player: {0}")
            await client.write_gatt_char(CharacteristicUUID.current_player.value, int_to_bytes(0))
            print(f"Timer: {30000}")
            await client.write_gatt_char(CharacteristicUUID.timer.value, int_to_bytes(30000))
            print(f"Skipped: {0}")
            await client.write_gatt_char(CharacteristicUUID.skipped.value, bool_to_bytes(False))
            print(f"Game active: {0}")
            await client.write_gatt_char(CharacteristicUUID.game_active.value, bool_to_bytes(False))
        print("Set up clients")

        await asyncio.sleep(5)

        print("Starting game")
        #Start game
        await write_to_all_clients(CharacteristicUUID.game_active.value, bool_to_bytes(True))

        print("Started game")
        await asyncio.sleep(1)

        # For 5 turns
        for j in range(5):
            print(f"Turn {j + 1}")
            for i, client in enumerate(clients):
                if await client.read_gatt_char(CharacteristicUUID.skipped.value):
                    print(f"Skipping: {i}")
                print(f"Client: {i}")
                current_player = i
                await write_to_all_clients(CharacteristicUUID.current_player.value, int_to_bytes(current_player))
                await client.write_gatt_char(CharacteristicUUID.my_turn.value, bool_to_bytes(True))

                while True:
                    value = int.from_bytes(await client.read_gatt_char(CharacteristicUUID.my_turn.value), byteorder="little")
                    if not value:
                        break




            # Simulated turns
            for i in range(len(clients) - 1, num_players - len(clients)):
                current_player = i + len(clients)
                await write_to_all_clients(CharacteristicUUID.current_player.value, int_to_bytes(current_player))
                print(f"Simming turn {i}")
                await aioconsole.ainput()


"""
    clients: List[BleakClient] = []
    for device in game_devices:
        clients.append(BleakClient(device, services=[SERVICE_UUID]))

    for i, client in enumerate(clients):
        print(f"Connecting to client {i} {client.address}")
        print(await client.connect(timeout=60))
        print(f"Connected to client 0")

    try:
        print(f"Setting up clients: {clients}")
        # Setup clients
        for i, client in enumerate(clients):
            print(f"Player Number: {i} | {CharacteristicUUID.my_player}")
            await client.write_gatt_char(CharacteristicUUID.my_player, i)
            print(f"Player turn  : {0}")
            await client.write_gatt_char(CharacteristicUUID.my_turn, 0)
            print(f"Number player: {num_players}")
            await client.write_gatt_char(CharacteristicUUID.num_players, num_players)
            print(f"current Player: {0}")
            await client.write_gatt_char(CharacteristicUUID.current_player, 0)
            print(f"Timer: {30000}")
            await client.write_gatt_char(CharacteristicUUID.timer, 30000)
            print(f"Skipped: {0}")
            await client.write_gatt_char(CharacteristicUUID.skipped, 0)
            print(f"Game active: {0}")
            await client.write_gatt_char(CharacteristicUUID.game_active, 0)
        print("Set up clients")

        await asyncio.sleep(5)

        print("Starting game")
        #Start game
        # await write_to_all_clients(CharacteristicUUID.game_active, 1)

        print("Started game")
        await asyncio.sleep(1)

        # For 5 turns
        for i in range(5):
            print(f"Turn {i + 1}")
            current_player = i
            for i, client in enumerate(clients):
                if await client.read_gatt_char(CharacteristicUUID.skipped):
                    print(f"Skipping: {i}")
                print(f"Client: {i}")
                await write_to_all_clients(CharacteristicUUID.current_player, current_player)
                await client.write_gatt_char(CharacteristicUUID.my_turn, 1)

            # Simulated turns
            for i in range(len(clients), num_players - len(clients)):
                current_player = i + len(clients)
                await write_to_all_clients(CharacteristicUUID.current_player, current_player)
                print(f"Simming turn {i}")
                await aioconsole.ainput()

    finally:
        for client in clients:
            await client.disconnect()
    """
asyncio.run(main())

"""
import simplepyble
import time
def game_device_filter(peripheral):
    return peripheral.address() == "48:27:e2:e1:5a:f1"

if __name__ == "__main__":
    adapters = simplepyble.Adapter.get_adapters()

    if len(adapters) == 0:
        print("No adapters found")

    adapter: simplepyble.Adapter = adapters[0]

    adapter.set_callback_on_scan_start(lambda: print("Scan started."))
    adapter.set_callback_on_scan_stop(lambda: print("Scan complete."))
 
    # Scan for 5 seconds
    adapter.scan_for(5000)
    peripherals = adapter.scan_get_results()

    peripheral = [peripheral for peripheral in peripherals if peripheral.identifier() == "FISCHER1"][0]
    print(peripheral)
    peripheral.connect()    
    services = peripheral.services()
    for service in services:
        print(f"Service: {service.uuid()}")
        for characteristic in service.characteristics():
            print(f"    Characteristic: {characteristic.uuid()}")

            capabilities = " ".join(characteristic.capabilities())
            print(f"    Capabilities: {capabilities}")
    peripheral.disconnect()
"""