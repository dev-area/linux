# Import required python libraries
from flask import Flask
import numpy as np
import pandas as pd
# API definition
app = Flask(__name__)

@app.route('/model/datagenerator_v1.0', methods=['POST'])
def predict_p1_model1_1():
    cols = ['feature_1', 'feature_2', 'feature_3', 'feature_4', 'feature_5']
    feature_1 = np.random.randint(low=100, high=999, size=50)
    feature_2 = np.random.randint(low=700, high=900, size=50)
    feature_3 = np.random.randint(low=10, high=99, size=50)
    feature_4 = np.random.randint(low=1000, high=9999, size=50)
    feature_5 = np.random.randint(low=1, high=10, size=50)
    
    real_estate_dataset = pd.DataFrame({'feature_1': feature_1,
                           'feature_2': feature_2,
                           'feature_3': feature_3,
                           'feature_4': feature_4,
                           'feature_5': feature_5 })[cols]
    return real_estate_dataset.to_json()

            
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5802, debug=True)