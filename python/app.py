

def run(graphics):
    print('-', dir(graphics))

    print(graphics.hello())
    print("- Hello from Python")

    if graphics.CreateDot():
        print("nice")

    BlueCake = graphics.Cake()
    BlueCake.Eat()
