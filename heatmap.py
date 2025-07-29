import numpy as np
import matplotlib.pyplot as plt

# Parameters
a, b, c, d = 2, 3, 2, 3

# Define the function
def f(x, y):
    return (x**a) * ((1 - x)**b) * (y**c) * ((1 - y)**d)

# Generate grid
x = np.linspace(0, 1, 300)
y = np.linspace(0, 1, 300)
X, Y = np.meshgrid(x, y)
Z = f(X, Y)

# Plot heatmap
plt.figure(figsize=(6, 5))
plt.imshow(Z, extent=[0, 1, 0, 1], origin='lower', cmap='viridis', aspect='auto')
plt.colorbar(label='f(x, y)')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Heatmap of f(x, y) = x^a * (1-x)^b * y^c * (1-y)^d')
plt.tight_layout()
plt.show()