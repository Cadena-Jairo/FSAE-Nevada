import can

def send_message(data, test_flag = False):

    # If not all inputs are integers
    for d in data:
        if type(d) != int:
            return -1
    
    # If empty input
    if len(data) == 0:
        return -1

    msg = can.Message(
        arbitration_id=0xC0FFEE, 
        data=data, 
        is_extended_id=True
    )
    if test_flag:
        return msg
    else:
        with can.Bus() as bus:
       
            try:
                bus.send(msg)
                print(f"Message sent on {bus.channel_info}")
            except can.CanError:
                print("Message NOT sent")
        return None


def parse_can_message(message, test_flag = False):
    """Parse a CAN message and extract relevant information."""
    id = hex(message.arbitration_id)
    data = message.data

    # Example: Assuming the first two bytes are temperature and pressure
    if len(data) >= 2:
        temperature = data[0]  # First byte
        pedal1 = data[1]     # Second byte
        pedal2 = data[2]     # Second byte
        print(f"Temperature: {temperature} °C, Pedal1_Voltage: {pedal1} V, Pedal2_Voltage: {pedal2} V")

    print(f"Message ID: {id}, Data: {data.hex()}, Timestamp: {message.timestamp}")
    return data




if __name__ == '__main__':
    test_flag = True # Set in order to conduct tests
    data = [80, 8, 8] # Inputs from system

    print("\nENCODER")
    msg = send_message(data, test_flag)
    print(msg)

    print("\nDECODER")
    parse_can_message(msg, test_flag)
    print()