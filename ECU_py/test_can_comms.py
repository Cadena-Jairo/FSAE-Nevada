import unittest
from can_comms import send_message, parse_can_message

class Testing(unittest.TestCase):

    def test_sendcan_data_type(self):
        """
        Tests if pass in empty data
        """
        test_flag = True # Stops methods from attempting to connect to CANbus
        data_input = [80.1, 80, 10.01] # Inputs from system
        msg = send_message(data_input, test_flag)
        
        self.assertEqual(-1, msg)

    def test_sendcan_empty_data(self):
        """
        Tests if pass in empty data
        """
        test_flag = True # Stops methods from attempting to connect to CANbus
        data_input = [] # Inputs from system
        msg = send_message(data_input, test_flag)

        self.assertEqual(-1, msg)


    def test_data_input_output_length(self):
        """
        Tests if input and output are equal in length
        """
        test_flag = True # Stops methods from attempting to connect to CANbus
        data_input = [80, 8, 8] # Inputs from system

        msg = send_message(data_input, test_flag)
        data_output = parse_can_message(msg, test_flag)
        
        self.assertEqual(len(data_input), len(data_output))
        
    def test_data_integrity(self):
        """
        Ensures data input is the same as the data that's output
        """
        test_flag = True # Stops methods from attempting to connect to CANbus
        data_input = [80, 8, 8] # Inputs from system

        msg = send_message(data_input, test_flag)
        data_output = parse_can_message(msg, test_flag)
        
        val = True
        for i in range(len(data_input)):
            if data_input[i] != data_output[i]:
                val *= False
        self.assertEqual(True, val)

        

if __name__ == '__main__':
    unittest.main()


