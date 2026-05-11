from flask import Flask, render_template, request, render_template_string
import html

app = Flask(__name__)

# FLAG = "BITs2CTF{SST1_1nj3ct10n_4_Th3_W1n}"

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')

@app.route('/chat', methods=['POST'])
def chat():
    user_input = request.form.get('message', '')
    blacklist = ["config", "_", "read", "{{", "d", "<"]

    if any([n in user_input.lower() for n in blacklist]):
        user_input = "**"
    
    template = """
    <div class="chat-message user-message">
        <div class="message-content">
            <strong>You:</strong> {{ user_input }}
        </div>
    </div>""" + f"""
    <div class="chat-message bot-message">
        <div class="message-content">
            <strong>DeepSleep:</strong> {user_input}？
        </div>
    </div>
    """
    
    return render_template_string(template, user_input=user_input)

if __name__ == '__main__':
    app.run(debug=False) 