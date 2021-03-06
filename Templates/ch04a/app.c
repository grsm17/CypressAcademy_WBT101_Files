#include "app_bt_cfg.h"
#include "sparcommon.h"
#include "wiced_bt_dev.h"
#include "wiced_platform.h"
#include "wiced_bt_trace.h"
#include "wiced_hal_puart.h"
#include "wiced_bt_stack.h"
#include "wiced_rtos.h"

#include "cycfg.h"

/* TODO Ex 01, Ex 02: Include header files */

/*******************************************************************
 * Macros to assist development of the exercises
 ******************************************************************/
 
/* Convenient defines for thread sleep times */
#define SLEEP_10MS		(10)
#define SLEEP_100MS		(100)
#define SLEEP_250MS		(250)
#define SLEEP_1000MS	(1000)

/* PWM configuration defines */
#define PWM_FREQUENCY	(1000)

/* PWM range values */
#define PWM_MAX			(0xFFFF)
#define PWM_INIT		(PWM_MAX-999)

/* PWM compare values for blinking, always on, always off */
#define PWM_TOGGLE		(PWM_MAX-100)
#define PWM_ALWAYS_ON	(PWM_INIT)
#define PWM_ALWAYS_OFF	(PWM_MAX)


/*******************************************************************
 * Function Prototypes
 ******************************************************************/
static wiced_bt_dev_status_t	app_bt_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data );
static wiced_bt_gatt_status_t	app_gatt_callback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data );

wiced_bt_gatt_status_t			app_gatt_get_value( wiced_bt_gatt_attribute_request_t *p_attr );
wiced_bt_gatt_status_t			app_gatt_set_value( wiced_bt_gatt_attribute_request_t *p_attr );

void							app_set_advertisement_data( void );


/*******************************************************************
 * Global/Static Variables
 ******************************************************************/

/* TODO Ex 02: Add global variables */


/*******************************************************************************
* Function Name: void application_start( void )
********************************************************************************/
void application_start( void )
{
    #if ((defined WICED_BT_TRACE_ENABLE) || (defined HCI_TRACE_OVER_TRANSPORT))
        /* Select Debug UART setting to see debug traces on the appropriate port */
        wiced_set_debug_uart( WICED_ROUTE_DEBUG_TO_PUART );
    #endif

    wiced_bt_trace_enable( );
    WICED_BT_TRACE( "**** CYW20819 App Start **** \r\n" );


    /* Initialize Stack and Register Management Callback */
    wiced_bt_stack_init( app_bt_management_callback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools );
}


/*******************************************************************************
* Function Name: wiced_bt_dev_status_t app_bt_management_callback(
* 					wiced_bt_management_evt_t event,
* 					wiced_bt_management_evt_data_t *p_event_data )
********************************************************************************/
wiced_result_t app_bt_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_result_t status = WICED_BT_SUCCESS;

    switch( event )
    {
		case BTM_ENABLED_EVT:								// Bluetooth Controller and Host Stack Enabled
			if( WICED_BT_SUCCESS == p_event_data->enabled.status )
			{
				WICED_BT_TRACE( "Bluetooth Enabled\r\n" );

				/* Print out the local Bluetooth Device Address. The address type  is set in the makefile (BT_DEVICE_ADDRESS) */
				wiced_bt_device_address_t bda;
				wiced_bt_dev_read_local_addr( bda );
				WICED_BT_TRACE( "Local Bluetooth Device Address: [%B]\r\n", bda );

				/* TODO Ex 01, Ex 02: Register GATT callback and initialize the GATT database */


				/* TODO Ex 01, Ex 02: Enable/disable pairing */

				
                /* TODO Ex 02: Configure PWM */


				/* Create the packet and begin advertising */
				app_set_advertisement_data();
				wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL );
			}
			else
			{
				WICED_BT_TRACE( "Bluetooth Disabled\r\n" );
			}
			break;

		case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT: 		// IO capabilities request
			break;

		case BTM_PAIRING_COMPLETE_EVT: 						// Pairing Complete event
			break;

		case BTM_ENCRYPTION_STATUS_EVT: 						// Encryption Status Event
			break;

		case BTM_SECURITY_REQUEST_EVT: 						// Security access
			break;

		case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT: 			// Save link keys with app
			break;

		case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT: 		// Retrieval saved link keys
			break;

		case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT: 				// Save keys to NVRAM
			break;

		case  BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT: 			// Read keys from NVRAM
			break;

		case BTM_BLE_SCAN_STATE_CHANGED_EVT: 					// Scan State Change
			break;

		case BTM_BLE_ADVERT_STATE_CHANGED_EVT:					// Advertising State Change
            WICED_BT_TRACE( "Advertising state = %d\r\n", p_event_data->ble_advert_state_changed );
            /* TODO Ex 02: Set PWM to appropriate duty cycle */
			break;

		default:
			WICED_BT_TRACE( "Unhandled Bluetooth Management Event: 0x%x (%d)\n", event, event );
			break;
    }

    return status;
}


/*******************************************************************************
* Function Name: wiced_bt_gatt_status_t app_gatt_callback(
* 					wiced_bt_gatt_evt_t event,
* 					wiced_bt_gatt_event_data_t *p_data )
********************************************************************************/
wiced_bt_gatt_status_t app_gatt_callback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data )
{
    wiced_bt_gatt_status_t result = WICED_SUCCESS;

    wiced_bt_gatt_connection_status_t *p_conn = &p_data->connection_status;
    wiced_bt_gatt_attribute_request_t *p_attr = &p_data->attribute_request;

    switch( event )
    {
        case GATT_CONNECTION_STATUS_EVT:					// Remote device initiates connect/disconnect
            if( p_conn->connected )
			{
				WICED_BT_TRACE( "GATT_CONNECTION_STATUS_EVT: Connect BDA %B, Connection ID %d\r\n",p_conn->bd_addr, p_conn->conn_id );
				
				/* TODO Ex 02: Handle the connection */
			}
			else
			{
				// Device has disconnected
				WICED_BT_TRACE( "GATT_CONNECTION_STATUS_EVT: Disconnect BDA %B, Connection ID %d, Reason=%d\r\n", p_conn->bd_addr, p_conn->conn_id, p_conn->reason );
				
				/* TODO Ex 02: Handle the disconnection */

				/* Restart the advertisements */
				wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL );
			}
            break;

        case GATT_ATTRIBUTE_REQUEST_EVT:					// Remote device initiates a GATT read/write
			switch( p_attr->request_type )
			{
				case GATTS_REQ_TYPE_READ:
					result = app_gatt_get_value( p_attr );
					break;

				case GATTS_REQ_TYPE_WRITE:
					result = app_gatt_set_value( p_attr );
					break;
            }
            break;

        default:
            WICED_BT_TRACE( "Unhandled GATT Event: 0x%x (%d)\r\n", event, event );
            break;
    }

    return result;
}



/*******************************************************************************
* Function Name: void app_set_advertisement_data( void )
********************************************************************************/
void app_set_advertisement_data( void )
{
    wiced_bt_ble_advert_elem_t adv_elem[2] = { 0 };
    uint8_t adv_flag = BTM_BLE_GENERAL_DISCOVERABLE_FLAG | BTM_BLE_BREDR_NOT_SUPPORTED;
    uint8_t num_elem = 0;

    /* Advertisement Element for Flags */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len = sizeof( uint8_t );
    adv_elem[num_elem].p_data = &adv_flag;
    num_elem++;

    /* Advertisement Element for Name */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len = app_gap_device_name_len;
    adv_elem[num_elem].p_data = app_gap_device_name;
    num_elem++;

    /* Set Raw Advertisement Data */
    wiced_bt_ble_set_raw_advertisement_data( num_elem, adv_elem );
}


/*******************************************************************************
* Function Name: app_gatt_get_value(
* 					wiced_bt_gatt_attribute_request_t *p_attr )
********************************************************************************/
wiced_bt_gatt_status_t app_gatt_get_value( wiced_bt_gatt_attribute_request_t *p_attr )
{
	uint16_t attr_handle = 	p_attr->data.handle;
	uint8_t  *p_val = 		p_attr->data.read_req.p_val;
	uint16_t *p_len = 		p_attr->data.read_req.p_val_len;

	int i = 0;
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;

    // Check for a matching handle entry
    for (i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == attr_handle)
        {
            // Detected a matching handle in the external lookup table
            if (app_gatt_db_ext_attr_tbl[i].cur_len <= *p_len)
            {
                // Value fits within the supplied buffer; copy over the value
                *p_len = app_gatt_db_ext_attr_tbl[i].cur_len;
                memcpy(p_val, app_gatt_db_ext_attr_tbl[i].p_data, app_gatt_db_ext_attr_tbl[i].cur_len);
                res = WICED_BT_GATT_SUCCESS;

                // TODO Ex 01: Add code for any action required when this attribute is read
                switch ( attr_handle )
                {
//					case handle:
//						break;
                }
            }
            else
            {
                // Value to read will not fit within the buffer
                res = WICED_BT_GATT_INVALID_ATTR_LEN;
            }
            break;
        }
    }

    return res;
}


/*******************************************************************************
* Function Name: app_gatt_set_value(
*					wiced_bt_gatt_attribute_request_t *p_attr )
********************************************************************************/
wiced_bt_gatt_status_t app_gatt_set_value( wiced_bt_gatt_attribute_request_t *p_attr )
{
	uint16_t attr_handle = 	p_attr->data.handle;
	uint8_t  *p_val = 		p_attr->data.write_req.p_val;
	uint16_t len = 			p_attr->data.write_req.val_len;

	int i = 0;
    wiced_bool_t validLen = WICED_FALSE;
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;

    // Check for a matching handle entry
    for (i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == attr_handle)
        {
            // Detected a matching handle in external lookup table
            // Verify that size constraints have been met
            validLen = (app_gatt_db_ext_attr_tbl[i].max_len >= len);
            if (validLen)
            {
                // Value fits within the supplied buffer; copy over the value
                app_gatt_db_ext_attr_tbl[i].cur_len = len;
                memcpy(app_gatt_db_ext_attr_tbl[i].p_data, p_val, len);
                res = WICED_BT_GATT_SUCCESS;

                // TODO Ex 01: Add code for any action required when this attribute is written
                // For example you may need to write the value into NVRAM if it needs to be persistent
                switch ( attr_handle )
                {
//					case handle:
//						break;
                }
            }
            else
            {
                // Value to write does not meet size constraints
                res = WICED_BT_GATT_INVALID_ATTR_LEN;
            }
            break;
        }
    }

    return res;
}