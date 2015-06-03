<?php

/**
 * @file
 * Functions for use in the form_test module.
 */

/**
 * @file
 * Loads data xml into an array.
 * @return array
 */
function form_test_load_xml() {
  // Load xml file into array
  if (file_exists(drupal_get_path('module', 'form_test') . '/drupal_7.xml')) {
    $xml = simplexml_load_file(drupal_get_path('module', 'form_test') . '/drupal_7.xml');
  }
  // Use json encode/decode to easily convert object into array.
  $data = drupal_json_decode(drupal_json_encode($xml));

  return $data;
}

/**
 * @file
 * Generates form_test_xmlform.
 * @return array
 */
function form_test_xmlform(array $data) {
  $data = form_test_load_xml();

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form defined in xml'),
  );
  // Define fieldset
  $fieldset = $data['fieldset']['title'];
  $form[$fieldset] = array(
    '#type' => 'fieldset',
    '#title' => t($data['fieldset']['title']),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Loop each field and create it
  foreach ($data['fieldset']['fields']['field'] as $field) {
    //Handle different field types
    switch ($field['type']) {
      case 'text':
        form_test_build_text_field($field, $fieldset, $form);
        break;
      case 'textarea':
        form_test_build_textarea_field($field, $fieldset, $form);
        break;
      case 'date':
        form_test_build_date_field($field, $fieldset, $form);
        break;
      case 'select':
        form_test_build_select_field($field, $fieldset, $form);
        break;
      case 'radio':
        form_test_build_radio_field($field, $fieldset, $form);
        break;
      case 'checkbox':
        form_test_build_check_box_field($field, $fieldset, $form);
        break;
      default:
        // Do nothing in this case
        break;
    }
  }
  // Add submit button
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save'),
    '#executes_submit_callback' => TRUE,
  ];
  // Add custom validation
  $form['#validate'][] = 'form_test_xmlform_validate';
  // Add custom submition
  $form['#submit'][] = 'form_test_xmlform_submit';
  return $form;
}

/**
 * @file
 * Generates text field array for form api.
 * @param $field
 * @param $fieldset
 * @param $form
 * @return array
 */
function form_test_build_text_field($field, $fieldset, &$form) {
  // Set field values
  $field_type = 'textfield';
  $field_maxlen = $field['maxlen'];
  $field_name = $field['name'];
  $field_label = $field['label'];
  $field_default_value = $field['default_value'];
  $field_required = $field['required'] ? TRUE : FALSE;
  // Build text field.
  $form[$fieldset][$field_name] = array(
    '#type' => $field_type,
    '#title' => t($field_label),
    '#required' => $field_required,
    '#default_value' => $field_default_value,
    '#size' => $field_maxlen,
    '#maxlength' => $field_maxlen,
    '#element_validate' => ['validate_field'],
  );
  // Add max length if text field
  if ($field_type == 'text') {
    $form[$fieldset][$field_name]['#size'] = $field_maxlen;
    $form[$fieldset][$field_name]['#maxlength'] = $field_maxlen;
  }
}

/**
 * @file
 * Generates textarea field array for form api.
 * @param $field
 * @param $fieldset
 * @param $form
 * @return array
 */
function form_test_build_textarea_field($field, $fieldset, &$form) {
  // Set field values
  $field_type = $field['type'];
  $field_name = $field['name'];
  $field_label = $field['label'];
  $field_required = $field['required'] ? TRUE : FALSE;
  // Build text field.
  $form[$fieldset][$field_name] = [
    '#type' => $field_type,
    '#title' => t($field_label),
    '#required' => $field_required,
    '#value' => '',
  ];
  // Add max length if text field
  if ($field_type == 'text') {
    $form[$fieldset][$field_name]['#size'] = $field_maxlen;
    $form[$fieldset][$field_name]['#maxlength'] = $field_maxlen;
  }
}

/**
 * @file
 * Generates date field array for form api.
 * @param $field
 * @param $fieldset
 * @param $form
 * @return array
 */
function form_test_build_date_field($field, $fieldset, &$form) {
  // Set field values
  $field_type = $field['type'];
  $field_name = $field['name'];
  $field_label = $field['label'];
  $field_default_value = $field['default_value'];
  $field_required = $field['required'] ? TRUE : FALSE;
  // format date string for default value into array.
  $date_parts = explode('-', $field_default_value);
  $default_date = [
    'year' => $date_parts[0],
    'month' => $date_parts[1],
    'day' => $date_parts[2]
  ];
  // Build date field
  $form[$fieldset][$field_name] = [
    '#type' => $field_type,
    '#title' => t($field_label),
    '#default_value' => $default_date,
    '#required' => $field_required,
  ];
}

/**
 * @file
 * Generates select field array for form api.
 * @param $field
 * @param $fieldset
 * @param $form
 * @return array
 */
function form_test_build_select_field($field, $fieldset, &$form) {
  // Set field values
  $field_type = $field['type'];
  $field_name = $field['name'];
  $field_label = $field['label'];
  $field_default_value = $field['default_value'];
  $field_options = $field['options']['option'];
  $field_required = $field['required'] ? TRUE : FALSE;
  // Define options array
  $options = [];
  foreach ($field_options as $option) {
    $options[$option['value']] = t($option['text']);
  }
  // Build select field
  $form[$fieldset][$field_name] = [
    '#type' => $field_type,
    '#title' => t($field_label),
    '#options' => $options,
    '#default_value' => $field_default_value,
    '#required' => $field_required,
  ];
}

/**
 * @file
 * Generates radios field array for form api.
 * @param $field
 * @param $fieldset
 * @param $form
 * @return array
 */
function form_test_build_radio_field($field, $fieldset, &$form) {
  // Set field values
  $field_type = 'radios';
  $field_name = $field['name'];
  $field_label = $field['label'];
  $field_description = $field['description'];
  $field_options = $field['options']['option'];
  $field_required = $field['required'] ? TRUE : FALSE;
  // Define options array
  $options = [];
  foreach ($field_options as $option) {
    $options[] = t($option);
  }
  // Build radio field
  $form[$fieldset][$field_name] = [
    '#type' => $field_type,
    '#title' => t($field_label),
    '#default_value' => 0,
    '#options' => $options,
    '#description' => t($field_description),
  ];
}

/**
 * @file
 * Generates check box field array for form api.
 * @param $field
 * @param $fieldset
 * @param $form
 * @return array
 */
function form_test_build_check_box_field($field, $fieldset, &$form) {
  // Set field values
  $field_type = $field['type'];
  $field_name = $field['name'];
  $field_label = $field['label'];
  $field_description = $field['description'];
  $field_required = $field['required'] ? TRUE : FALSE;
  // Build check box field
  $form[$fieldset][$field_name] = [
    '#type' => $field_type,
    '#title' => t($field_label),
    '#required' => $field_required,
    '#description' => t($field_description),
  ];
}
