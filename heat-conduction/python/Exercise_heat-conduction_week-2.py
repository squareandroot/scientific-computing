import numpy as np
import matplotlib.pyplot as plt
from scipy.linalg import solveh_banded
from tabulate import tabulate
import timeit

# =============================================================================
# T_n+1[i] = T_n[i] + delta_t/delta_x^2 (T_n[i+1] - 2 * T_n[i] + T_n[i-1])
# The cake is a trap.
# =============================================================================


def evaluateExplicitly(x_steps, time_steps, initialValueFunction):
    delta_x = 1/(x_steps - 1)
    delta_t = 1/(time_steps - 1)

    temperature_matrix = np.zeros((time_steps, x_steps))
    temperature_matrix[0] = initialValueFunction(np.linspace(0, 1, x_steps))

    def explicitEuler(value_matrix):
        alpha = delta_t/delta_x**2

        for n in range(1, len(value_matrix)):
            value_matrix[n] = value_matrix[n-1] + alpha * (
                np.roll(value_matrix[n-1], -1)
                - 2*value_matrix[n-1]
                + np.roll(value_matrix[n-1], 1))

            value_matrix[n, 0] = value_matrix[0, 0]
            value_matrix[n, -1] = value_matrix[0, -1]

    explicitEuler(temperature_matrix)
    return temperature_matrix


def evaluateImplicitly(x_steps, time_steps, initialValueFunction):
    delta_x = 1/(x_steps - 1)
    delta_t = 1/(time_steps - 1)

    temperature_matrix = np.zeros((time_steps, x_steps))
    temperature_matrix[0] = initialValueFunction(np.linspace(0, 1, x_steps))

    def implicitEulerWithBandedSolve(value_matrix):
        alpha = delta_t/delta_x**2
        main_diagonal = np.full(len(value_matrix[0])-2, 1 + 2 * alpha)
        sub_diagonal = np.full(len(value_matrix[0])-3, -alpha)

        coefficient_matrix = np.zeros((2, len(value_matrix[0])-2))
        coefficient_matrix[0, 1:] = sub_diagonal
        coefficient_matrix[1] = main_diagonal

        boundary_conditions = np.zeros(len(value_matrix[0])-2)
        boundary_conditions[0] = alpha * value_matrix[0, 0]
        boundary_conditions[-1] = alpha * value_matrix[0, -1]

        for n in range(1, len(value_matrix)):
            value_matrix[n, 0] = value_matrix[0, 0]
            value_matrix[n, -1] = value_matrix[0, -1]

            constant_vector = value_matrix[n-1, 1:-1] + boundary_conditions

            next_step = solveh_banded(coefficient_matrix, constant_vector)
            value_matrix[n, 1:-1] = next_step

    implicitEulerWithBandedSolve(temperature_matrix)
    return temperature_matrix


def T0(x):
    return 350 - 100*x + 200*np.sin(np.pi*x)


def menu():
    print("Welcome! \n"
          + "- To see evalutation times enter 'time' \n"
          + "- To solve the heat equation and plot the results enter 'solve' \n"
          + "Terminate by entering 'exit'")
    menu_item = input("--> ")
    if menu_item == "solve":
        method_chosen = input("How do you want to solve the heat equation? " +
                              "Explicitly [E] or implicitly [I]? \n --> ")

        if method_chosen in ("E", "e"):
            x_steps = int(input("How many subdivisions in space? "
                                + "(Input an integer) --> "))
            time_steps = int(input("How many time steps? "
                                   + "(Input an integer) --> "))

            temperature_matrix = evaluateExplicitly(x_steps, time_steps, T0)
            np.savetxt("out-explicit.csv", temperature_matrix, delimiter=";")
            print(temperature_matrix)

            plot_chosen = input("Do you want to plot the result?\n"
                                + "Yes [Y] or No [N]? --> ")

            if plot_chosen in ("Y", "y"):
                x = np.linspace(0, 1, x_steps)
                t = np.linspace(0, 1, time_steps)

                X, Y = np.meshgrid(x, t)
                Z = temperature_matrix

                ax = plt.axes(projection='3d')
                ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
                                cmap='plasma', edgecolor='none')
                ax.view_init(22.432, 63.689)
                ax.set_xlabel("x'")
                ax.set_ylabel("t'")
                ax.set_zlabel("Temperature [K]")
                ax.set_title("Explicit Euler")
                # plt.show()
                plt.savefig("explicit.pdf")

                menu()

            else:
                print("Restart \n--------------------------")
                menu()

        elif method_chosen in ("I", "i"):

            x_steps = int(input("How many subdivisions in space? "
                                + "(Input an integer) --> "))
            time_steps = int(input("How many time steps? "
                                   + "(Input an integer) --> "))

            temperature_matrix = evaluateImplicitly(x_steps, time_steps, T0)
            np.savetxt("out-implicit.csv", temperature_matrix, delimiter=";")
            print(temperature_matrix)

            plot_chosen = input("Do you want to plot the result?\n"
                                + "Yes [Y] or No [N]? --> ")

            if plot_chosen in ("Y", "y"):
                x = np.linspace(0, 1, x_steps)
                t = np.linspace(0, 1, time_steps)

                X, Y = np.meshgrid(x, t)
                Z = temperature_matrix

                ax = plt.axes(projection='3d')
                ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
                                cmap='plasma', edgecolor='none')
                ax.view_init(22.432, 63.689)
                ax.set_xlabel("x'")
                ax.set_ylabel("t'")
                ax.set_zlabel("Temperature [K]")
                ax.set_title("Implicit Euler")
                # plt.show()
                plt.savefig("implicit.pdf")

                menu()

            else:
                print("Restart \n--------------------------")
                menu()

        else:
            print("Restart \n--------------------------")
            menu()

    elif menu_item == "exit":
        return None

    elif menu_item == "time":
        timeFunctions()
        menu()

    else:
        print("Restart \n--------------------------")
        menu()


def timeFunctions():
    setup = '''
import numpy as np
from scipy.linalg import solve, solveh_banded

x_steps = 10
time_steps = 200

delta_x = 1/(x_steps - 1)
delta_t = 1/(time_steps - 1)


def T0(x):
        return 350 - 100*x + 200*np.sin(np.pi*x)


temperature_matrix = np.zeros((time_steps, x_steps))
temperature_matrix[0] = T0(np.linspace(0, 1, x_steps))


def implicitEulerWithSolve(value_matrix):
    main_diagonal = np.full(len(value_matrix[0])-2, 1 + 2 * delta_t / delta_x**2)
    sub_diagonal = np.full(len(value_matrix[0])-3, -delta_t / delta_x**2)
    
    coefficient_matrix = np.diag(main_diagonal) + np.diag(sub_diagonal, k=-1) + np.diag(sub_diagonal, k=1)
    
    boundary_conditions = np.zeros(len(value_matrix[0])-2)
    boundary_conditions[0] = delta_t / delta_x**2 * value_matrix[0, 0]
    boundary_conditions[-1] = delta_t / delta_x**2 * value_matrix[0, -1]
    
    for n in range(1, len(value_matrix)):
        value_matrix[n, 0] = value_matrix[0, 0]
        value_matrix[n, -1] = value_matrix[0, -1]
        
        constant_vector = value_matrix[n-1, 1:-1] + boundary_conditions
        
        next_step = solve(coefficient_matrix, constant_vector)
        value_matrix[n, 1:-1] = next_step
    
    return value_matrix


def implicitEulerWithBandedSolve(value_matrix):
    main_diagonal = np.full(len(value_matrix[0])-2, 1 + 2 * delta_t / delta_x**2)
    sub_diagonal = np.full(len(value_matrix[0])-3, -delta_t / delta_x**2)
    
    coefficient_matrix = np.zeros((2, len(value_matrix[0])-2))
    coefficient_matrix[0, 1:] = sub_diagonal
    coefficient_matrix[1] = main_diagonal
    
    boundary_conditions = np.zeros(len(value_matrix[0])-2)
    boundary_conditions[0] = delta_t / delta_x**2 * value_matrix[0, 0]
    boundary_conditions[-1] = delta_t / delta_x**2 * value_matrix[0, -1]
    
    for n in range(1, len(value_matrix)):
        value_matrix[n, 0] = value_matrix[0, 0]
        value_matrix[n, -1] = value_matrix[0, -1]
        
        constant_vector = value_matrix[n-1, 1:-1] + boundary_conditions
        
        next_step = solveh_banded(coefficient_matrix, constant_vector)
        value_matrix[n, 1:-1] = next_step
    
    return value_matrix


def explicitEuler(value_matrix):
    alpha = delta_t/delta_x**2
    
    for n in range(1, len(value_matrix)):
        value_matrix[n] = value_matrix[n-1] + alpha * (
            np.roll(value_matrix[n-1], -1)
            - 2*value_matrix[n-1]
            + np.roll(value_matrix[n-1], 1))
        
        value_matrix[n, 0] = value_matrix[0, 0]
        value_matrix[n, -1] = value_matrix[0, -1]
    
    return value_matrix
'''

    iterations = 100

    t1 = timeit.Timer("implicitEulerWithSolve(temperature_matrix)", setup)
    t_implicit = t1.timeit(iterations)

    t1 = timeit.Timer(
        "implicitEulerWithBandedSolve(temperature_matrix)", setup)
    t_implicit_banded = t1.timeit(iterations)

    t1 = timeit.Timer("explicitEuler(temperature_matrix)", setup)
    t_explicit = t1.timeit(iterations)

    data = [["Implicit: scipy.solve", t_implicit/iterations],
            ["Implicit: scipy.solveh_banded", t_implicit_banded/iterations],
            ["Explicit", t_explicit/iterations]]

    header = ["Method", "Avg time after {} iterations in sec".format(iterations)]

    print(tabulate(data, header, tablefmt="fancy_grid"))


menu()
