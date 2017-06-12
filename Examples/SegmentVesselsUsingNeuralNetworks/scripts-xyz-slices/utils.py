from glob import glob
import json
import numpy as np
import os
import sys
import shutil

import keras.models
import sqlite3

class Logger(object):

    def __init__(self, log_file):
        self.terminal = sys.stdout
        self.log = open(log_file, "w")

    def write(self, message):
        self.terminal.write(message)
        self.log.write(message)

    def flush(self):
        #this flush method is needed for python 3 compatibility.
        #this handles the flush command by doing nothing.
        #you might want to specify some extra behavior here.
        pass


try:
    with open(os.path.join(os.path.dirname(__file__), 'params.json')) as f:
        script_params = json.load(f)
except (IOError, ValueError):
    import params
    script_params = params.params.copy()

def set_params_path(path):
    with open(path) as f:
        new_params = json.load(f)
    # Let "from utils import script_params" work
    script_params.clear()
    script_params.update(new_params)

def ensureDirectoryExists(path):
    """Create the directory named by path and any necessary parents if it
    doesn't exist.

    """
    if not os.path.exists(path):
        os.makedirs(path)


def symlink_entries_through(source, dest, *args):
    for arg in args:
        symlink_through(os.path.join(source, arg), os.path.join(dest, arg))

def symlink_through(source, dest):
    """Create a symlink at dest to source, or what source links to if
    source is itself a symlink.

    """
    if os.path.islink(source):
        source = readlink_absolute(source)
    os.symlink(os.path.relpath(source, os.path.dirname(dest)), dest)

def readlink_absolute(path):
    rl = os.readlink(path)
    if not os.path.isabs(rl):
        rl = os.path.normpath(os.path.join(os.path.dirname(path), rl))
    return rl

def open_sqlite3_db(dir):
    """Open the sqlite3 database contained in dir.  We use "data.sqlite3"."""
    return sqlite3.connect(os.path.join(dir, 'data.sqlite3'))

def scale_net_input_data(data):
    """Convert to float64"""
    return data.astype(float)

def pad(im):
    """Pad im with BACKGROUND_PADDING in the amount of PATCH_RADIUS on
    each edge of each dimension so that it can be passed to
    extractPatch

    """
    return np.pad(im, script_params['PATCH_RADIUS'], 'reflect')

def extractPatch(im_padded, indices):
    """Return a patch extracted from im_padded at indices.

    Indices are for the corresponding unpadded image.

    If NETWORK_DESIGN is "xyz", this means returning an array of size
    (2W+1 x 2W+1 x 3), where W is the patch radius.

    If NETWORK_DESIGN is "full3d", this means returning an array of
    size (2W+1 x 2W+1 x 2W+1).

    """
    im_p = im_padded
    w = script_params['PATCH_RADIUS']
    design = script_params['NETWORK_DESIGN']
    if design == 'xyz':
        # Return N (N-1)-dimensional slices
        return np.stack((im_p[tuple(np.s_[x : x + 2*w + 1] if i != j else x + w
                                    for j, x in enumerate(indices))]
                         for i in range(len(indices))),
                        axis=-1)
    elif design == 'full3d':
        # Return an N-dimensional slice
        return im_p[tuple(np.s_[x : x + 2*w + 1] for x in indices)]
    else:
        raise ValueError('Unknown NETWORK_DESIGN')

def separateChannels(im):
    """Transpose an ...xN image into an Nx...x1 image"""
    return np.moveaxis(im, -1, 0)[..., np.newaxis]

def prepareInputArray(im):
    """For NETWORK_DESIGN == "xyz", convert a Bx...xC array to a list of C
    Bx...x1 arrays, converting data types appropriately in the
    process.

    For NETWORK_DESIGN == "full3d", only the data type is converted.

    """
    design = script_params['NETWORK_DESIGN']
    if design == 'xyz':
        return list(scale_net_input_data(separateChannels(im)))
    elif design == 'full3d':
        return scale_net_input_data(im[..., np.newaxis])
    else:
        raise ValueError('Unknown NETWORK_DESIGN')

def best_model_path():
    return os.path.join(script_params['OUTPUT_DATA_ROOT'], "NetProto", "net_best.hdf5")

def load_best_model():
    return keras.models.load_model(best_model_path())

def predict_on_indices(model, input_image_padded, indices, batch_size):
    """Run prediction on patches taken from input_image_padded centered at
    the various indices (which are relative to the unpadded image)

    """
    predictions = []
    for i in range(0, len(indices), batch_size):
        ind = indices[i:i + batch_size]
        patches = prepareInputArray(np.stack(
            extractPatch(input_image_padded, i) for i in ind
        ))
        pred = model.predict_on_batch(patches)[:, 1]
        predictions.append(pred)
        # Print progress
        print '\t %.2f%%' % (100.0 * (i + len(pred)) / len(indices)),
        print "%.4f, %.4f, %.4f" % (pred.min(), pred.max(), pred.mean())
    return np.concatenate(predictions)

def original_image(name_key):
    r, = glob(os.path.join(script_params['INPUT_DATA_ROOT'],
                           '*',
                           script_params['TYPE_SUBDIR_STRUCTURE'],
                           name_key + '.mh[ad]'))
    return r
