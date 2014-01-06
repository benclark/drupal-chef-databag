<?php

/**
 * @file
 * Entity API classes for chef_databag module.
 */


/**
 * The class used for chef_databag_item entities.
 */
class ChefDatabagItem extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'chef_databag_item');
  }

  protected function defaultLabel() {
    return $this->id;
  }

  protected function defaultUri() {
    return array('path' => 'chef_databag_item/' . $this->drupal_id);
  }

}

/**
 * The class used for chef_databag entities.
 */
class ChefDatabag extends Entity {

  public $name;
  public $weight;

  public function __construct($values = array()) {
    parent::__construct($values, 'chef_databag');
  }

}

/**
 * The Controller for chef_databag_item entities.
 */
class ChefDatabagItemController extends EntityAPIController {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a chef_databag_item.
   *
   * @return
   *   A chef_databag_item object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += array(
      'drupal_id' => '',
      'id' => '',
      'data_bag' => '',
      'language' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'data' => '',
    );
    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Save a chef_databag_item entity.
   */
  public function save($entity) {
    $entity->changed = REQUEST_TIME;
    $result = parent::save($entity);

    // Convert the entity to an array that will become JSON.
    $data = $this->view_json($entity, FALSE);

    // Allow the json-prepped data to be altered.
    // @see chef_encrypted_databag_chef_databag_save_alter()
    drupal_alter('chef_databag_save', $data, $entity);

    // Check whether this data bag item already exists on Chef server.
    try {
      if (chef_knife('data/' . $entity->data_bag . '/' . $entity->id)) {
        // Already exists, so update.
        chef_knife('data/' . $entity->data_bag . '/' . $entity->id, 'PUT', $data);
      }
    }
    catch (Exception $e) {
      // Create a new data bag item object on Chef server.
      chef_knife('data/' . $entity->data_bag, 'POST', $data);
    }

    return $result;
  }

  /**
   * Format entity for inclusion in JSON output.
   */
  public function view_json($entity, $output = FALSE) {
    $return = array(
      'id' => $entity->id,
      'drupal_id' => $entity->drupal_id,
      'created' => date('c', $entity->created),
      'changed' => date('c', $entity->changed),
    );

    field_attach_prepare_view('chef_databag_item', array($entity->drupal_id => $entity), 'full', $entity->language);
    entity_prepare_view('chef_databag_item', array($entity->drupal_id => $entity), $entity->language);
    $content = field_attach_view('chef_databag_item', $entity, 'view', $entity->language);

    foreach ($content as $key => $field_data) {
      if (strpos($key, 'field_') === 0) {
        // Get the formatted data for the field.
        $return[$key] = $this->view_json_field($field_data);
      }
    }

    return ($output) ? drupal_json_output($return) : $return;
  }

  /**
   * Format fields for inclusion in JSON output.
   */
  private function view_json_field($field_data) {
    $return = array();

    switch ($field_data['#field_type']) {
      case 'list_boolean':
        foreach ($field_data['#items'] as $key => $value) {
          $return[$key] = (bool) $value['value'];
        }
        break;

      default:
        foreach ($field_data['#items'] as $key => $value) {
          // If this field value does not have a #markup rendered value, use
          // the raw value in the #items array.
          if (!isset($field_data[$key]['#markup'])) {
            $return[$key] = $value['value'];
          }
          else {
            // Prefer the #markup rendered value.
            $return[$key] = $field_data[$key]['#markup'];
          }
        }
        break;
    }

    // If there is only one item in the array, and it is indexed '0' then just
    // return the singleton value.
    return (!isset($return[0]) || count($return) > 1) ? $return : reset($return);
  }
}

/**
 * The Controller for chef_databag entities.
 */
class ChefDatabagController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a chef_databag.
   *
   * @return
   *   A chef_databag object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Model
    $values += array(
      'drupal_id' => '',
      'name' => '',
      'weight' => 0,
      'data' => '',
    );
    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Save a chef_databag entity.
   */
  public function save($entity) {
    $result = parent::save($entity);

    // Check whether this data_bag already exists on Chef server.
    try {
      if (chef_knife('data/' . $entity->name)) {
        // Already exists, nothing to do.
      }
    }
    catch (Exception $e) {
      // Create a new data_bag object on Chef server.
      chef_knife('data', 'POST', array('name' => $entity->name));
    }

    return $result;
  }
}
