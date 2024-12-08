import argparse
import re

def update_macros(file_path, led_pin, res_type):
    # Determine LED_ON and LED_OFF based on connection type
    if res_type.lower() == "pu":
        led_on, led_off = 0, 1
    elif res_type.lower() == "pd":
        led_on, led_off = 1, 0
    else:
        print("Error: Resistor type must be 'pu' (pull-up) or 'pd' (pull-down).")
        return

    # Define the macro patterns to search for
    macros = {
        "LED_PIN": f"#define LED_PIN GPIO_NUM_{led_pin}",
        "LED_ON": f"#define LED_ON {led_on}",
        "LED_OFF": f"#define LED_OFF {led_off}",
    }

    try:
        # Read the contents of the file
        with open(file_path, "r") as file:
            content = file.readlines()

        # Update macros in the file
        for i, line in enumerate(content):
            for macro, replacement in macros.items():
                if line.strip().startswith(f"#define {macro}"):
                    content[i] = replacement + "\n"

        # Write the updated content back to the file
        with open(file_path, "w") as file:
            file.writelines(content)
        print("Macros updated successfully!")

    except FileNotFoundError:
        print(f"Error: source file main.c in main directory was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

def main():
    parser = argparse.ArgumentParser(
        description="Updates LED-related macros in project's main source file."
    )
    parser.add_argument(
        "led_pin",
        type=int,
        help="LED GPIO pin number (default for ESP32 DEVKIT V1 - 2)."
    )
    parser.add_argument(
        "res_type",
        type=str,
        choices=["pu", "pd"],
        help="Resistor type ('pu' for pull-up, 'pd' for pull-down)."
    )

    args = parser.parse_args()

    update_macros("main/main.c", args.led_pin, args.res_type)

if __name__ == "__main__":
    main()
