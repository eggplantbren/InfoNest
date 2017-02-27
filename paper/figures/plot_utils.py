import matplotlib.pyplot as plt

def rc():
    """
    Sets plotting defaults.
    """
    plt.rc("font", size=14, family="serif", serif="Computer Sans")
    plt.rc("text", usetex=True)

