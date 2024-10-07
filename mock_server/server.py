import aioconsole
import asyncio
import threading
from bleak import BleakScanner, BleakClient
from bleak.backends.device import BLEDevice
from bleak.uuids import normalize_uuid_16
from enum import Enum
from typing import List

SERVICE_UUID = "d7560343-51d4-4c24-a0fe-118fd9078144"

class CharacteristicUUID(str, Enum):
    num_players = normalize_uuid_16(0x1)
    current_player = normalize_uuid_16(0x2)
    timer = normalize_uuid_16(0x3)
    my_player = normalize_uuid_16(0x4)
    my_turn = normalize_uuid_16(0x5)
    skipped = normalize_uuid_16(0x6)
    game_active = normalize_uuid_16(0x7)


async def main():
    async def write_to_all_clients(uuid: CharacteristicUUID, value: int):
        for client in clients:
            await client.write_gatt_char(uuid, value)
            


    num_players = 8
    current_player = 0



    game_devices = [device for device in await BleakScanner.discover(service_uuids = [SERVICE_UUID])]
    print(game_devices)

    async with BleakClient(game_devices[0], timeout=60, services=[SERVICE_UUID]) as client:
        clients = [client]
        print(f"Setting up client: {client}")
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
        await write_to_all_clients(CharacteristicUUID.game_active, 1)

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
