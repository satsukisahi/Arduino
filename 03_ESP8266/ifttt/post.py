import requests

response = requests.post(
        'https://api.beebotte.com/v1/data/publish/esp8266/led?token=token_PsM3oFwVwjVS4Rrz',
        {"data":31500})