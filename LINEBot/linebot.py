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

line_bot_api = LineBotApi('access_token') 
handler = WebhookHandler('channel_secret') 


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


global a
a=0
@app.route("/ToI", methods=["GET"])
def handle_get_request():
    global a
    
    b = a
    a = 0
    print("accept_"+str(b))
    line_bot_api.push_message("user_id",TextSendMessage(text="accept_"+str(b)))
    
    return "%%%"+str(b)+"%%%"



@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    text = event.message.text
    global a
    
    dry = ["今日じめじめしとるなぁ、除湿しとくな","除湿まかしとき！"]
    if text == "ToI! 除湿して!":
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=dry[np.random.randint(2)]))
        a = 1
        

    cool = ["ええよー、冷しとくで！","今日暑かったもんなぁ、しっかり冷やしとくわ！"]
    elif text == "ToI! 冷房して!":
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=cool[np.random.randint(2)]))
        a = 2

    else:
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=text))


if __name__ == "__main__":
    port = int(os.environ.get('PORT', 8000))
    app.run(host ='0.0.0.0',port = port)