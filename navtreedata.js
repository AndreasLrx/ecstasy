/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "ECSTASY", "index.html", [
    [ "ECSTASY (Entity Component System Toward Architecture Saving Years)", "index.html", null ],
    [ "Building", "md_doc_2_building.html", null ],
    [ "Glossary", "md_doc_2_glossary.html", [
      [ "Entity", "md_doc_2_glossary.html#autotoc_md15", null ],
      [ "Component", "md_doc_2_glossary.html#autotoc_md16", null ],
      [ "Storage", "md_doc_2_glossary.html#autotoc_md17", null ],
      [ "System", "md_doc_2_glossary.html#autotoc_md18", null ],
      [ "Resource", "md_doc_2_glossary.html#autotoc_md19", null ],
      [ "Registry", "md_doc_2_glossary.html#autotoc_md20", null ],
      [ "Query", "md_doc_2_glossary.html#autotoc_md21", [
        [ "Queryable", "md_doc_2_glossary.html#autotoc_md22", null ],
        [ "Batch Query", "md_doc_2_glossary.html#autotoc_md23", null ],
        [ "Modifier", "md_doc_2_glossary.html#autotoc_md24", null ],
        [ "Condition", "md_doc_2_glossary.html#autotoc_md25", null ]
      ] ],
      [ "Thread", "md_doc_2_glossary.html#autotoc_md26", [
        [ "Lockable", "md_doc_2_glossary.html#autotoc_md27", null ]
      ] ],
      [ "Serialization", "md_doc_2_glossary.html#autotoc_md28", [
        [ "Serializer actions", "md_doc_2_glossary.html#autotoc_md29", [
          [ "Export", "md_doc_2_glossary.html#autotoc_md30", null ],
          [ "Import", "md_doc_2_glossary.html#autotoc_md31", null ]
        ] ],
        [ "Variable actions", "md_doc_2_glossary.html#autotoc_md32", [
          [ "Serialize: Save", "md_doc_2_glossary.html#autotoc_md33", null ],
          [ "Deserialize: Update", "md_doc_2_glossary.html#autotoc_md34", null ],
          [ "Deserialize: Load", "md_doc_2_glossary.html#autotoc_md35", null ]
        ] ]
      ] ]
    ] ],
    [ "Tutorial", "md_doc_2_tutorial.html", [
      [ "Getting Started", "md_doc_2_tutorial.html#GettingStarted", null ],
      [ "Using entities", "md_doc_2_tutorial.html#autotoc_md37", [
        [ "Creating entities", "md_doc_2_tutorial.html#autotoc_md38", [
          [ "Using type resolution", "md_doc_2_tutorial.html#autotoc_md39", null ]
        ] ],
        [ "Manage entity component", "md_doc_2_tutorial.html#autotoc_md40", null ],
        [ "Delete entities", "md_doc_2_tutorial.html#autotoc_md41", null ]
      ] ],
      [ "Implement a custom storage", "md_doc_2_tutorial.html#CustomStorageTutorial", null ],
      [ "Making registry queries", "md_doc_2_tutorial.html#QueryTutorial", [
        [ "Select ... Where ...", "md_doc_2_tutorial.html#SelectWhereTutorial", null ],
        [ "Using modifiers", "md_doc_2_tutorial.html#ModifiersTutorial", null ],
        [ "Using conditions", "md_doc_2_tutorial.html#ConditionsTutorial", null ]
      ] ],
      [ "Using systems", "md_doc_2_tutorial.html#autotoc_md42", null ],
      [ "Using resources", "md_doc_2_tutorial.html#autotoc_md43", null ],
      [ "Ensuring Thread Safety", "md_doc_2_tutorial.html#EnsuringThreadSafety", null ],
      [ "Using cross platform RTTI", "md_doc_2_tutorial.html#autotoc_md44", [
        [ "Why use custom RTTI implementation", "md_doc_2_tutorial.html#autotoc_md45", null ],
        [ "How to use it", "md_doc_2_tutorial.html#autotoc_md46", [
          [ "How is it stored", "md_doc_2_tutorial.html#autotoc_md47", null ],
          [ "Registering types", "md_doc_2_tutorial.html#autotoc_md48", null ],
          [ "Querying types", "md_doc_2_tutorial.html#autotoc_md49", null ]
        ] ]
      ] ],
      [ "Serializing your entities", "md_doc_2_tutorial.html#autotoc_md50", [
        [ "Using a serializer", "md_doc_2_tutorial.html#autotoc_md51", [
          [ "Common types", "md_doc_2_tutorial.html#autotoc_md52", null ],
          [ "Custom types", "md_doc_2_tutorial.html#autotoc_md53", null ],
          [ "Working with Entities", "md_doc_2_tutorial.html#autotoc_md54", null ]
        ] ]
      ] ]
    ] ],
    [ "Technical Implementations", "md_doc_2_technical_implementation.html", [
      [ "Entities Management", "md_doc_2_technical_implementation.html#autotoc_md56", null ],
      [ "Components Management (MapStorage)", "md_doc_2_technical_implementation.html#autotoc_md57", null ],
      [ "Query", "md_doc_2_technical_implementation.html#autotoc_md58", null ]
    ] ],
    [ "Events integration", "md_doc_2integrations_2events.html", [
      [ "Description", "md_doc_2integrations_2events.html#autotoc_md67", [
        [ "How to get feedback", "md_doc_2integrations_2events.html#autotoc_md68", null ],
        [ "Event flow", "md_doc_2integrations_2events.html#autotoc_md69", null ]
      ] ],
      [ "Example", "md_doc_2integrations_2events.html#autotoc_md70", [
        [ "Creating listeners components", "md_doc_2integrations_2events.html#autotoc_md71", null ],
        [ "Sending an event", "md_doc_2integrations_2events.html#autotoc_md72", null ],
        [ "Accessing an input state", "md_doc_2integrations_2events.html#autotoc_md73", null ]
      ] ]
    ] ],
    [ "SFML integration", "md_doc_2integrations_2sfml.html", [
      [ "Description", "md_doc_2integrations_2sfml.html#autotoc_md75", null ],
      [ "Usage", "md_doc_2integrations_2sfml.html#autotoc_md76", null ]
    ] ],
    [ "User Actions integration", "md_doc_2integrations_2user__actions.html", [
      [ "Description", "md_doc_2integrations_2user__actions.html#autotoc_md78", null ],
      [ "Usage", "md_doc_2integrations_2user__actions.html#autotoc_md79", [
        [ "Register your actions and bindings", "md_doc_2integrations_2user__actions.html#autotoc_md80", null ],
        [ "Listen to your actions", "md_doc_2integrations_2user__actions.html#autotoc_md81", [
          [ "ActionListener", "md_doc_2integrations_2user__actions.html#autotoc_md82", null ],
          [ "ActionListeners", "md_doc_2integrations_2user__actions.html#autotoc_md83", null ],
          [ "ActionIdListener", "md_doc_2integrations_2user__actions.html#autotoc_md84", null ]
        ] ],
        [ "Using intermediate actions", "md_doc_2integrations_2user__actions.html#autotoc_md85", null ],
        [ "Saving and loading the bindings", "md_doc_2integrations_2user__actions.html#autotoc_md86", [
          [ "Saving", "md_doc_2integrations_2user__actions.html#autotoc_md87", null ],
          [ "Loading", "md_doc_2integrations_2user__actions.html#autotoc_md88", null ]
        ] ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"01__basic__window_2main_8cpp.html",
"_shared_recursive_mutex_8hpp.html",
"classecstasy_1_1_instances.html#a60a0a52241c58dd76d10144fba5c8307",
"classecstasy_1_1_registry_1_1_registry_stack_query_memory_3_01util_1_1meta_1_1_traits_3_01_selecfcfaca0c668d0d83b5ea7096197c8168.html#ae6a0979483eb1a06e84922357f00ca28",
"classecstasy_1_1integration_1_1event_1_1_gamepad.html#af0f4687925fe97b9b703f92253c2e571aebcc5da4a444a0a699ceab62d93abc35",
"classecstasy_1_1integration_1_1sfml_1_1_encoding.html#af2f3d430d9bd4d27e094d50eb35f058a",
"classecstasy_1_1query_1_1modifier_1_1_and_impl.html#a896ac70a187fcec57f854fc575b21a29",
"classecstasy_1_1serialization_1_1_json_serializer.html#a20be92bab0cddd1ec02702e4eff8d972",
"classutil_1_1_bit_set.html#a0314c01283427177909d5977641ec6f1",
"classutil_1_1serialization_1_1_i_node.html#a605ed86fc1c6e533484ce54f63628f00",
"classutil_1_1serialization_1_1_toml_conversion.html#a5680f5a5f3c594b6c1a34abde1eedc72",
"cpp/algorithm/ranges/find.html",
"cpp/experimental/new_delete_resource.html",
"cpp/io/manip/unitbuf.html",
"cpp/numeric/special_functions/cyl_neumann.html",
"cpp/types/is_swappable.html",
"functions_r.html",
"registry_2modifiers_2_not_8hpp.html",
"structecstasy_1_1integration_1_1user__action_1_1_action_id_listener.html#a0edb68a032d82442850df4c482623189",
"structutil_1_1meta_1_1_traits_3_01_arg_00_01_args_8_8_8_01_4.html#a37457752027ce8ceb510051e784df526"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';