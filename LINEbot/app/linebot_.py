from flask import Flask, request, abort

from linebot import (
    LineBotApi, WebhookHandler
)
from linebot.exceptions import (
    InvalidSignatureError
)
from linebot.models import (
    MessageEvent, TextMessage, TextSendMessage,
)
import os, json 
import time
import numpy as np

app = Flask(__name__)

line_bot_api = LineBotApi('QfrdNbYR/rBaZEw2RQ4dayc9uq15yEBhssFC76MmOcAHUyaCjr7FdSQ7MgeqSEas3L16ABEqTe2XEX/s2dIdq9G9INms3CdZHFnCbAPCMOaNeOxpi3zN/3avuyprEiXDM1yNFHn1YLTLp4+zJ3+C1gdB04t89/1O/w1cDnyilFU=') #アクセストークンを入れてください
handler = WebhookHandler('97b93e7a4d467cc7ea117921c5bf81fc') #Channel Secretを入れてください


@app.route("/callback", methods=['POST'])
def callback():

    signature = request.headers['X-Line-Signature']

    body = request.get_data(as_text=True)
    app.logger.info("Request body: " + body)
 
    try: 
        handler.handle(body, signature)
    except InvalidSignatureError:
        abort(400)

    return 'OK'


# @app.route("/dehumidification", methods=['POST'])
# def push_humidity():

#     #byte型で送られてくるので、strに変換
#     data = request.data.decode()
    
#     print(data,type(data))
#     text = "ただいまの湿度は" + data + "%デス!"

#     line_bot_api.push_message("U0e4dbe5d87211d5f5a79aafae89808dc",TextSendMessage(text=text))

#     return "OK"


global a
a=0
@app.route("/ToI", methods=["GET"])
def handle_get_request():
    global a
    
    b = a
    a = 0
    print("accept_"+str(b))
    line_bot_api.push_message("U0e4dbe5d87211d5f5a79aafae89808dc",TextSendMessage(text="accept_"+str(b)))
    
    return "%%%"+str(b)+"%%%"


dry = ["今日じめじめしとるなぁ、除湿しとくな","除湿まかしとき！"]
cool = ["ええよー、冷しとくで！","今日暑かったもんなぁ、しっかり冷やしとくわ！"]
@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    text = event.message.text
    global a
    
    
    if text == "ToI! 除湿して!":
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=dry[np.random.randint(2)]))
        a = 1
        

    elif text == "ToI! 冷房して!":
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=cool[np.random.randint(2)]))
        a = 2

    else:
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=text))


if __name__ == "__main__":
    port = int(os.environ.get('PORT', 8000))
    app.run(host ='0.0.0.0',port = port)