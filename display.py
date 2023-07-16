import matplotlib.pyplot as plt
import json

# Read JSON data from file
with open("json.data") as file:
    data = json.load(file)

# Extract language and lines data from JSON
languages = [entry['language'] for entry in data.values()]
lines = [entry['lines'] for entry in data.values()]

# Create a bar chart
plt.bar(languages, lines)
plt.xlabel('Languages')
plt.ylabel('Lines')
plt.title('Language Lines Bar Chart')
plt.xticks(rotation=90)  # Rotate x-axis labels for better visibility

# Display the chart
plt.tight_layout()
plt.show()
