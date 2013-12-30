<?php

/**
 * @file
 * Administrative functions and callbacks for chef_databag module.
 */


/**
 * Admin UI controller for chef_databag_item entities.
 */
class ChefDatabagItemUIController extends EntityBundleableUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';

    // Fix the admin content path.
    unset($items[$this->path . '/list']);
    $items['admin/content/chef_databag_item'] = array(
      'title' => $plural_label,
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_overview_form', $this->entityType),
      'description' => 'Manage ' . $plural_label . '.',
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType),
      'file' => 'includes/entity.ui.inc',
      'file path' => drupal_get_path('module', 'entity'),
      'type' => MENU_LOCAL_TASK,
    );

    // Cleaning up cruft added by EntityDefaultUIController.
    unset($items[$this->path . '/manage/' . $wildcard]);
    unset($items[$this->path . '/manage/' . $wildcard . '/edit']);
    unset($items[$this->path . '/manage/' . $wildcard . '/clone']);
    unset($items[$this->path . '/manage/' . $wildcard . '/%']);
    return $items;
  }

  /**
   * Returns a single row in the admin overview table.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    // Fix the Edit link
    $uri = $entity->uri();
    $row[1] = l(t('edit'), $uri['path'] . '/edit', array('query' => drupal_get_destination()));

    // Fix the Delete link
    $row[2] = l(t('delete'), $uri['path'] . '/delete', array('query' => drupal_get_destination()));

    return $row;
  }
}

/**
 * Generates the chef_databag_item editing form.
 */
function chef_databag_item_form($form, &$form_state, $databag_item, $op = 'edit') {
  $form = array();

  $form['id'] = array(
    '#type' => 'machine_name',
    '#title' => t('ID'),
    '#description' => t('A unique machine-readable ID for this data_bag.'),
    '#required' => TRUE,
    '#default_value' => $databag_item->id,
    '#machine_name' => array(
      'exists' => 'chef_databag_item_exists',
      'replace_pattern' => '[^a-z0-9_\-]+',
      'replace' => '-',
      'error' => t('The machine-readable name must contain only lowercase letters, numbers, underscores, and hyphens.'),
    ),
    '#disabled' => ($op == 'edit'),
  );

  // Add the fields.
  field_attach_form('chef_databag_item', $databag_item, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the chef_databag_item form.
 */
function chef_databag_item_form_submit(&$form, &$form_state) {
  $databag_item = entity_ui_form_submit_build_entity($form, $form_state);
  $databag_item->save();
  $form_state['redirect'] = $databag_item->uri();
}

/**
 * Admin UI controller for chef_databag entities.
 */
class ChefDatabagUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Chef data_bags, including adding and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the chef_databag editing form.
 */
function chef_databag_form($form, &$form_state, $data_bag, $op = 'edit') {
  $form = array();

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Name'),
    '#description' => t('A unique machine-readable name for this data_bag.'),
    '#required' => TRUE,
    '#default_value' => $data_bag->name,
    '#machine_name' => array(
      'exists' => 'chef_databag_exists',
      'replace_pattern' => '[^a-z0-9_\-]+',
      'replace' => '-',
      'error' => t('The machine-readable name must contain only lowercase letters, numbers, underscores, and hyphens.'),
    ),
    '#disabled' => ($op == 'edit'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the chef_databag form.
 */
function chef_databag_form_submit(&$form, &$form_state) {
  $data_bag = entity_ui_form_submit_build_entity($form, $form_state);
  $data_bag->save();
  $form_state['redirect'] = 'admin/structure/chef_databag';
}
